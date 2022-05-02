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

CMAKE ?= cmake
CTEST ?= ctest
RM = $(CMAKE) -E rm -rf # force this one to use CMake

BUILDS ?= Builds

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

override LIMES_TESTS_DIR = $(patsubst %/,%,$(strip $(dir $(realpath $(firstword $(MAKEFILE_LIST))))))

#

.PHONY: help
help:  ## Print this message
	@grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(LIMES_TESTS_DIR)/Makefile | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

#

$(BUILDS):
	@cd $(LIMES_TESTS_DIR) && $(CMAKE) --preset tests

.PHONY: config
config: $(BUILDS) ## configure CMake

#

.PHONY: build
build: config ## runs CMake build
	@cd $(LIMES_TESTS_DIR) && $(CMAKE) --build --preset tests --config $(CONFIG)

#

.PHONY: test
test: build ## runs all tests
	@cd $(LIMES_TESTS_DIR) && $(CTEST) --preset default

#

.PHONY: clean
clean: ## Cleans the source tree
	@echo "Cleaning tests directory..."
	@cd $(LIMES_TESTS_DIR) && $(RM) $(BUILDS)
