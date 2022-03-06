
CONFIG ?= Release

# program aliases
RM = $(CMAKE) -E rm -rf # force this one to use CMake
CMAKE ?= cmake
CPACK ?= cpack
PRECOMMIT ?= pre-commit
GIT ?= git

# TO DO: CPACK_GENERATOR

# directory aliases
BUILDS ?= Builds
DOCS ?= doc
CACHE ?= Cache

ifeq ($(OS),Windows_NT)
	CMAKE_GENERATOR ?= Visual Studio 17 2022
else ifeq ($(shell uname -s),Darwin)
	CMAKE_GENERATOR ?= Xcode
	SUDO ?= sudo
else
	CMAKE_GENERATOR ?= Ninja
	SUDO ?= sudo
	export CC=gcc-10
	export CXX=g++-10
endif

#

override print_help = grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(THIS_MAKEFILE) | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

override precommit_init = $(PRECOMMIT) install --install-hooks --overwrite && $(PRECOMMIT) install --install-hooks --overwrite --hook-type commit-msg

override run_precommit = $(GIT) add . && $(PRECOMMIT) run --all-files

override run_clean = $(RM) $(BUILDS) $(DOCS); $(PRECOMMIT) gc

override run_wipe_cache = $(RM) $(CACHE); $(PRECOMMIT) clean

override run_uninstall = $(CMAKE) -P $(BUILDS)/uninstall.cmake

override cmake_configure_preset = $(CMAKE) --preset $(1) -G "$(CMAKE_GENERATOR)"

override cmake_default_configure = $(CMAKE) -B $(BUILDS) -G "$(CMAKE_GENERATOR)" -D CMAKE_BUILD_TYPE=$(CONFIG)

override cmake_build_preset = $(CMAKE) --build --preset $(1)

override cmake_default_build = $(CMAKE) --build $(BUILDS) --config $(CONFIG)

override cmake_install = $(SUDO) $(CMAKE) --install $(BUILDS) --config $(CONFIG) --strip --verbose

override cpack_create_installer = $(CMAKE) --build $(BUILDS) --target package
