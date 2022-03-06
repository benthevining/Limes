SHELL := /bin/sh
.ONESHELL:
.SHELLFLAGS: -euo
.DEFAULT_GOAL: help
.NOTPARALLEL:
.POSIX:

#

override LIMES_ROOT := $(patsubst %/,%,$(strip $(dir $(realpath $(firstword $(MAKEFILE_LIST))))))

override THIS_MAKEFILE := $(LIMES_ROOT)/Makefile

include $(LIMES_ROOT)/scripts/util.make

#

help:  ## Print this message
	@$(call print_help)

#

config: ## Configure cmake
	@cd $(LIMES_ROOT) && $(call cmake_default_configure)

build: config ## Builds the libraries
	@cd $(LIMES_ROOT) && $(call cmake_default_build)

install: build docs ## Runs CMake install
	@cd $(LIMES_ROOT) && $(call cmake_install)

pack: install ## Creates a CPack installer
	@cd $(LIMES_ROOT) && $(call cpack_create_installer)

#

config_ios: clean ## Configure iOS build [Mac only]
	@cd $(LIMES_ROOT) && $(call cmake_configure_preset,ios_simulator)

build_ios: config_ios ## Run iOS build [Mac only]
	@cd $(LIMES_ROOT) && $(call cmake_build_preset,ios_simulator)

#

docs: config ## Builds the documentation
	@cd $(LIMES_ROOT) && $(call cmake_default_build) --target LimesDoxygen

#

uninstall: ## Runs uninstall script [only works if project has been installed and was top-level project in configure]
	@$(call run_uninstall)

clean:  ## Cleans the source tree
	@echo "Cleaning..."
	@cd $(LIMES_ROOT) && $(call run_clean)


wipe: clean ## Wipes the persistent cache of fetched dependencies and ccache artifacts
	@echo "Wiping cache..."
	@cd $(LIMES_ROOT) && $(call run_wipe_cache)


init:  ## Initializes the workspace and installs all dependencies
	@cd $(LIMES_ROOT) && $(call precommit_init)


pc:  ## Runs all pre-commit hooks over all files
	@cd $(LIMES_ROOT) && $(call run_precommit)

#

.PHONY: $(shell grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_MAKEFILE) | sed 's/:.*/\ /' | tr '\n' ' ')
