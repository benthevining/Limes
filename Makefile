SHELL = /bin/sh
.ONESHELL:
.SHELLFLAGS: -euo
.DEFAULT_GOAL: help
.NOTPARALLEL:
.POSIX:

#

CONFIGS = Debug Release

CMAKE = cmake
PRECOMMIT = pre-commit
RM = rm -rf

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

BUILDS = Builds
CACHE = Cache

#

override LIMES_ROOT := $(patsubst %/,%,$(strip $(dir $(realpath $(firstword $(MAKEFILE_LIST))))))

override THIS_MAKEFILE := $(LIMES_ROOT)/Makefile

override make_build_dir = $(LIMES_ROOT)/util/$(1)/$(BUILDS)

override cmake_config = cd $(LIMES_ROOT) && $(CMAKE) -B $(BUILDS) -G "$(CMAKE_GENERATOR)" --log-level=DEBUG

override cmake_build_configuration = echo "Building $(1) configuration..."; $(CMAKE) --build $(BUILDS) -j $(NUM_CORES) --config $(1)

override cmake_build = $(foreach config,$(CONFIGS),$(call cmake_build_configuration,$(config));)

#

help:  ## Print this message
	@grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_MAKEFILE) | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

#

config: ## Configure cmake
	$(call cmake_config)

build: config ## Builds the libraries
	$(call cmake_build)

#

SCRIPTS_DIR = $(LIMES_ROOT)/scripts

clean:  ## Cleans the Lemons source tree
	$(RM) $(BUILDS)
	$(PRECOMMIT) gc


wipe: clean ## Wipes the persistent cache of fetched dependencies and ccache artifacts
	$(RM) $(CACHE)
	$(PRECOMMIT) clean


init:  ## Initializes the Lemons workspace and installs all dependencies
	@cd $(LIMES_ROOT) && $(PRECOMMIT) install --install-hooks --overwrite
	@cd $(LIMES_ROOT) && $(PRECOMMIT) install --install-hooks --overwrite --hook-type commit-msg


pc:  ## Runs all pre-commit hooks over all files
	@cd $(LIMES_ROOT) && git add . && $(PRECOMMIT) run --all-files

#

.PHONY: $(shell grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_MAKEFILE) | sed 's/:.*/\ /' | tr '\n' ' ')
