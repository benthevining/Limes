SHELL = /bin/sh
.ONESHELL:
.SHELLFLAGS: -euo
.DEFAULT_GOAL: help
.NOTPARALLEL:
.POSIX:

#

CACHE = Cache
BUILDS = Builds
CONFIGS = Debug Release

CMAKE = cmake
PRECOMMIT = pre-commit
RM = $(CMAKE) -E rm -rf
SUDO = sudo
GIT = git

ifeq ($(OS),Windows_NT)
	NUM_CORES = $(NUMBER_OF_PROCESSORS)
	CMAKE_GENERATOR = Visual Studio 17 2022
else ifeq ($(shell uname -s),Darwin)
	NUM_CORES = $(shell sysctl hw.ncpu | awk '{print $$2}')
	CMAKE_GENERATOR = Xcode
else
	NUM_CORES = $(shell grep -c ^processor /proc/cpuinfo)
	CMAKE_GENERATOR = Ninja
	export CC=gcc-10
	export CXX=g++-10
endif

# TO DO: CPACK_GENERATOR

#

override LIMES_ROOT := $(patsubst %/,%,$(strip $(dir $(realpath $(firstword $(MAKEFILE_LIST))))))

override THIS_MAKEFILE := $(LIMES_ROOT)/Makefile

override cmake_configure_preset = echo "Configuring $(1) preset..."; cd $(LIMES_ROOT) && $(CMAKE) --preset $(1) -G "$(CMAKE_GENERATOR)"

override cmake_build_preset = echo "Building $(1) preset..."; cd $(LIMES_ROOT) && $(CMAKE) --build --preset $(1)

override cmake_install_configuration = echo "Installing $(1) configuration..."; cd $(LIMES_ROOT) && $(SUDO) $(CMAKE) --install $(BUILDS)/$(1) --config $(1) --strip --verbose

override cmake_uninstall_configuration = echo "Uninstalling $(1) configuration..."; cd $(LIMES_ROOT) && $(CMAKE) -P $(BUILDS)/$(1)/uninstall.cmake

override cpack_create_installer = @cd $(ORANGES_ROOT) && $(CPACK) -G "$(CPACK_GENERATOR)" -C $(1) --verbose

#

help:  ## Print this message
	@grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_MAKEFILE) | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

#

config: ## Configure cmake
	@$(foreach config,$(CONFIGS),$(call cmake_configure_preset,$(config));)

build: config ## Builds the libraries
	@$(foreach config,$(CONFIGS),$(call cmake_build_preset,$(config));)

install: build ## Runs CMake install
	@$(foreach config,$(CONFIGS),$(call cmake_install_configuration,$(config));)

pack: install ## Creates a CPack installer
	@$(foreach config,$(CONFIGS),$(call cpack_create_installer,$(config));)

#

config_ios: clean ## Configure iOS build [Mac only]
	@$(call cmake_configure_preset,ios_simulator)

build_ios: config_ios ## Run iOS build [Mac only]
	@$(call cmake_build_preset,ios_simulator)

#

SCRIPTS_DIR = $(LIMES_ROOT)/scripts

uninstall: ## Runs uninstall script [only works if project has been installed and was top-level project in configure]
	@$(foreach config,$(CONFIGS),$(call cmake_uninstall_configuration,$(config));)

clean:  ## Cleans the Lemons source tree
	@echo "Cleaning..."
	@$(RM) $(LIMES_ROOT)/$(BUILDS)
	@$(PRECOMMIT) gc


wipe: clean ## Wipes the persistent cache of fetched dependencies and ccache artifacts
	@echo "Wiping cache..."
	@$(RM) $(CACHE)
	@$(PRECOMMIT) clean


init:  ## Initializes the Lemons workspace and installs all dependencies
	@cd $(LIMES_ROOT) && $(PRECOMMIT) install --install-hooks --overwrite
	@cd $(LIMES_ROOT) && $(PRECOMMIT) install --install-hooks --overwrite --hook-type commit-msg


pc:  ## Runs all pre-commit hooks over all files
	@cd $(LIMES_ROOT) && $(GIT) add . && $(PRECOMMIT) run --all-files

#

.PHONY: $(shell grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_MAKEFILE) | sed 's/:.*/\ /' | tr '\n' ' ')
