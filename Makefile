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

# directory aliases
BUILDS ?= Builds
DOCS ?= doc
CACHE ?= Cache
DEPS_GRAPH ?= deps_graph

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

#

.PHONY: help
help:  ## Print this message
	@grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(LIMES_ROOT)/Makefile | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

#

.PHONY: init
init:  ## Initializes the workspace and installs all dependencies
	@chmod +x $(LIMES_ROOT)/scripts/alphabetize_codeowners.py
	@cd $(LIMES_ROOT) && $(PRECOMMIT) install --install-hooks --overwrite && $(PRECOMMIT) install --install-hooks --overwrite --hook-type commit-msg

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

.PHONY: install
install: build ## runs CMake install
	@cd $(LIMES_ROOT) && $(SUDO) $(CMAKE) --install $(BUILDS) --config $(CONFIG)

.PHONY: pack
pack: build ## Creates a CPack installer
	@cd $(LIMES_ROOT) && $(CMAKE) --build $(BUILDS) --target package

#

.PHONY: test
test: build ## runs all tests
	@cd $(LIMES_ROOT) && $(CTEST) --preset default

#

.PHONY: pc
pc:  ## Runs all pre-commit hooks over all files
	@cd $(LIMES_ROOT) && $(GIT) add . && $(PRECOMMIT) run --all-files

#

.PHONY: deps_graph
deps_graph: config ## Generates a PNG image of the CMake dependency graph
	@cd $(LIMES_ROOT) && $(CMAKE) --build --preset deps_graph

.PHONY: docs
docs: config ## Builds the documentation
	@cd $(LIMES_ROOT) && $(CMAKE) --build --preset docs

#

.PHONY: uninstall
uninstall: ## Runs uninstall script
	@cd $(LIMES_ROOT) && $(SUDO) $(CMAKE) -P $(BUILDS)/uninstall.cmake

.PHONY: clean
clean: ## Cleans the source tree
	@echo "Cleaning..."
	@cd $(LIMES_ROOT)/tests && $(MAKE) clean
	@cd $(LIMES_ROOT) && $(RM) $(BUILDS) $(DOCS); $(PRECOMMIT) gc

.PHONY: wipe
wipe: ## Wipes the cache of downloaded dependencies
	@echo "Wiping cache..."
	@cd $(LIMES_ROOT) && $(SUDO) $(CMAKE) -P $(BUILDS)/wipe_cache.cmake
