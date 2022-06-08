SHELL := /bin/sh
.ONESHELL:
.SHELLFLAGS: -euo
.DEFAULT_GOAL: help
.NOTPARALLEL:
.POSIX:

#

CONFIG ?= Debug

export CMAKE_BUILD_TYPE ?= $(CONFIG)
export CMAKE_CONFIG_TYPE ?= $(CONFIG)
export VERBOSE ?= 1

# program aliases
CMAKE ?= cmake
CTEST ?= ctest
RM = $(CMAKE) -E rm -rf # force this one to use CMake
PRECOMMIT ?= pre-commit
GIT ?= git
PYTHON ?= python3
ASDF ?= asdf

#

# set default CMake generator & build parallel level
ifeq ($(OS),Windows_NT)
	export CMAKE_GENERATOR ?= Visual Studio 17 2022
	export CMAKE_BUILD_PARALLEL_LEVEL ?= $(NUMBER_OF_PROCESSORS)
else ifeq ($(shell uname -s),Darwin)
	export CMAKE_GENERATOR ?= Xcode
	export CMAKE_BUILD_PARALLEL_LEVEL ?= $(shell sysctl hw.ncpu | awk '{print $$2}')
	SUDO ?= sudo
else # Linux
	export CMAKE_GENERATOR ?= Ninja
	export CMAKE_BUILD_PARALLEL_LEVEL ?= $(shell grep -c ^processor /proc/cpuinfo)
	SUDO ?= sudo

	# use GCC 10 on Linux
	export CC=gcc-10
	export CXX=g++-10
endif

#

override LIMES_ROOT = $(patsubst %/,%,$(strip $(dir $(realpath $(firstword $(MAKEFILE_LIST))))))

override SCRIPTS = $(LIMES_ROOT)/scripts

override DOCS = $(LIMES_ROOT)/docs

BUILDS ?= $(LIMES_ROOT)/Builds
CACHE ?= $(LIMES_ROOT)/Cache

#

.PHONY: help
help:  ## Print this message
	@grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(LIMES_ROOT)/Makefile | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

#

.PHONY: init
init:  ## Initializes the workspace and installs all dependencies
	@chmod +x $(SCRIPTS)/build_all.sh $(SCRIPTS)/build_all_vecops_variants.sh
	@cd $(LIMES_ROOT) && \
		$(PRECOMMIT) install --install-hooks --overwrite && \
		$(PRECOMMIT) install --install-hooks --overwrite --hook-type commit-msg
	@cd $(LIMES_ROOT) && $(ASDF) install
	@$(PYTHON) -m pip install -r $(LIMES_ROOT)/requirements.txt

#

$(BUILDS):
	@cd $(LIMES_ROOT) && $(CMAKE) --preset maintainer

.PHONY: config
config: $(BUILDS) ## configure CMake

#

.PHONY: build
build: config ## runs CMake build
	@cd $(LIMES_ROOT) && $(CMAKE) --build --preset maintainer --config $(CONFIG)

#

.PHONY: all
all: clean ## Builds every configuration of every preset (may take a while)
	@time $(LIMES_ROOT)/scripts/build_all.sh

#

.PHONY: install
install: build ## runs CMake install
	$(SUDO) $(CMAKE) --install $(BUILDS) --config $(CONFIG)

.PHONY: pack
pack: build ## Creates a CPack installer
	$(CMAKE) --build $(BUILDS) --target package

#

.PHONY: test
test: build ## runs all tests
	@cd $(LIMES_ROOT) && $(CTEST) --preset default

.PHONY: qc
qc: ## Runs all qc scripts (takes a while!)
	$(SCRIPTS)/build_all.sh
	$(SCRIPTS)/build_all_vecops_variants.sh

#

.PHONY: pc
pc:  ## Runs all pre-commit hooks over all files
	@cd $(LIMES_ROOT) && $(GIT) add . && $(PRECOMMIT) run --all-files

#

.PHONY: deps_graph
deps_graph: config ## Generates a PNG image of the CMake dependency graph
	@cd $(DOCS) && $(MAKE) deps_graph

.PHONY: docs
docs: config ## Builds the documentation
	@cd $(DOCS) && $(MAKE) build

#

.PHONY: uninstall
uninstall: ## Runs uninstall script
	$(SUDO) $(CMAKE) -P $(BUILDS)/uninstall.cmake

.PHONY: clean
clean: ## Cleans the source tree
	@echo "Cleaning..."
	@cd $(LIMES_ROOT)/tests && $(MAKE) clean
	@cd $(DOCS) && $(MAKE) clean
	$(RM) $(BUILDS) logs deploy; $(PRECOMMIT) gc

.PHONY: wipe
wipe: clean ## Wipes the cache of downloaded dependencies
	@echo "Wiping cache..."
	$(RM) $(CACHE)
