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
override DOC = $(LIMES_ROOT)/doc
override BUILDS = $(LIMES_ROOT)/Builds

CACHE ?= $(LIMES_ROOT)/Cache

export FETCHCONTENT_BASE_DIR ?= $(CACHE)

#

.PHONY: help
help:  ## Print this message
	@$(call print_help,"$(LIMES_ROOT)/Makefile")

#

.PHONY: init
init:  ## Initializes the workspace and installs all dependencies
	@chmod +x $(SCRIPTS)/build_all.sh $(SCRIPTS)/build_all_vecops_variants.sh
	@$(PYTHON) -m pip install -r $(LIMES_ROOT)/requirements.txt
	@cd $(LIMES_ROOT) && \
		$(PRECOMMIT) install --install-hooks --overwrite && \
		$(PRECOMMIT) install --install-hooks --overwrite --hook-type commit-msg

#

$(BUILDS):
	@cd $(LIMES_ROOT) && $(CMAKE) --preset maintainer

.PHONY: config
config: $(BUILDS) ## configure CMake

#

.PHONY: open
open: config ## Opens the Limes project in an IDE
	$(CMAKE) --open $(BUILDS)

#

.PHONY: build
build: config ## runs CMake build
	@cd $(LIMES_ROOT) && $(CMAKE) --build --preset maintainer --config $(CONFIG)

#

$(BUILDS)/install_manifest.txt:
	$(SUDO) $(CMAKE) --install $(BUILDS) --config $(CONFIG)

.PHONY: install
install: build $(BUILDS)/install_manifest.txt ## runs CMake install

.PHONY: uninstall
uninstall: $(BUILDS)/install_manifest.txt ## Runs uninstall script
	$(SUDO) $(CMAKE) -P $(BUILDS)/uninstall.cmake

#

.PHONY: deps_graph
deps_graph: config ## Generates a PNG image of the CMake dependency graph
	$(CMAKE) --build $(BUILDS) --target LimesDependencyGraph

$(DOC): config
	$(CMAKE) --build $(BUILDS) --target LimesDocs

.PHONY: docs
docs: $(DOC) ## Builds the documentation

#

.PHONY: clean
clean: ## Cleans the source tree
	@echo "Cleaning..."
	# git clean -ffxd && git submodule foreach "git clean -ffxd
	$(RM) $(BUILDS) $(DOC)
	$(PRECOMMIT) gc

.PHONY: wipe
wipe: clean ## Wipes the cache of downloaded dependencies
	@echo "Wiping cache..."
	$(RM) $(CACHE) $(FETCHCONTENT_BASE_DIR)

#

.PHONY: pc
pc:  ## Runs all pre-commit hooks over all files
	@cd $(LIMES_ROOT) && $(GIT) add . && $(PRECOMMIT) run --all-files
