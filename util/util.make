CONFIG ?= Release

export CMAKE_BUILD_TYPE ?= $(CONFIG)
export CMAKE_CONFIG_TYPE ?= $(CONFIG)
export CMAKE_EXPORT_COMPILE_COMMANDS ?= 1
export VERBOSE ?= 1

# program aliases
CMAKE ?= cmake
CTEST ?= ctest
RM = $(CMAKE) -E rm -rf  # force this one to use CMake
PRECOMMIT ?= pre-commit
GIT ?= git
ASDF ?= asdf
PYTHON ?= python3

# set default CMake generator & build parallel level
ifeq ($(OS),Windows_NT)
	export CMAKE_GENERATOR ?= Visual Studio 17 2022
	export CMAKE_BUILD_PARALLEL_LEVEL ?= $(NUMBER_OF_PROCESSORS)
else ifeq ($(shell uname -s),Darwin)
	export CMAKE_GENERATOR ?= Xcode
	export CMAKE_BUILD_PARALLEL_LEVEL ?= $(shell sysctl hw.ncpu | sed -e "s/^hw.ncpu://")
	SUDO ?= sudo
else # Linux
	export CMAKE_GENERATOR ?= Ninja
	export CMAKE_BUILD_PARALLEL_LEVEL ?= $(shell grep -c ^processor /proc/cpuinfo)
	SUDO ?= sudo

	# use GCC 10 on Linux
	export CC=gcc-10
	export CXX=g++-10
endif

define print_help
	grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(1) | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
endef
