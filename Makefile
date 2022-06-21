SHELL := /bin/sh
.ONESHELL:
.SHELLFLAGS: -euo
.DEFAULT_GOAL: help
.NOTPARALLEL:
.POSIX:

#

override LIMES_ROOT = $(patsubst %/,%,$(strip $(dir $(realpath $(firstword $(MAKEFILE_LIST))))))

include $(LIMES_ROOT)/util/util.make

override SCRIPTS = $(LIMES_ROOT)/scripts
override DOCS = $(LIMES_ROOT)/docs
override BUILDS = $(LIMES_ROOT)/Builds
override CACHE = $(LIMES_ROOT)/Cache

#

.PHONY: help
help:  ## Print this message
	@$(call print_help,"$(LIMES_ROOT)/Makefile")

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

.PHONY: open
open: config ## Opens the Limes project in an IDE
	$(CMAKE) --open $(BUILDS)

#

.PHONY: build
build: config ## runs CMake build
	@cd $(LIMES_ROOT) && $(CMAKE) --build --preset maintainer --config $(CONFIG)

.PHONY: all
all: clean ## Builds every configuration of every preset (may take a while)
	@time $(LIMES_ROOT)/scripts/build_all.sh

.PHONY: qc
qc: ## Runs all qc scripts (takes a while!)
	$(SCRIPTS)/build_all.sh
	$(SCRIPTS)/build_all_vecops_variants.sh

#

$(BUILDS)/install_manifest.txt:
	$(SUDO) $(CMAKE) --install $(BUILDS) --config $(CONFIG)

.PHONY: install
install: build ## runs CMake install

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
	@cd $(DOCS) && $(MAKE) deps_graph

.PHONY: docs
docs: config ## Builds the documentation
	@cd $(DOCS) && $(MAKE) build

#

.PHONY: uninstall
uninstall: $(BUILDS)/install_manifest.txt ## Runs uninstall script
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
