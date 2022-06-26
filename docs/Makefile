SHELL := /bin/sh
.ONESHELL:
.SHELLFLAGS: -euo
.DEFAULT_GOAL: help
.NOTPARALLEL:
.POSIX:

#

# program aliases
CMAKE ?= cmake
RM = $(CMAKE) -E rm -rf # force this one to use CMake

#

override LIMES_DOCS = $(patsubst %/,%,$(strip $(dir $(realpath $(firstword $(MAKEFILE_LIST))))))

LIMES_ROOT ?= $(LIMES_DOCS)/..

BUILDS ?= $(LIMES_DOCS)/Builds
DOC ?= $(LIMES_ROOT)/doc

#

.PHONY: help
help:  ## Print this message
	@grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(LIMES_DOCS)/Makefile | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

#

$(BUILDS):
	$(CMAKE) -S $(LIMES_DOCS) -B $(BUILDS)

.PHONY: config
config: $(BUILDS) ## Configure CMake

#

$(DOC): config
	$(CMAKE) --build $(BUILDS) --target LimesDocs

.PHONY: build
build: $(DOC) ## Build the docs

#

.PHONY: deps_graph
deps_graph: config ## Build the dependency graph image
	$(CMAKE) --build $(BUILDS) --target LimesDependencyGraph

#

.PHONY: clean
clean: ## Clean the source tree
	@echo "Cleaning docs directory..."
	@$(RM) $(BUILDS) $(DOC)
