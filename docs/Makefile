SHELL := /bin/sh
.ONESHELL:
.SHELLFLAGS: -euo
.DEFAULT_GOAL: help
.NOTPARALLEL:
.POSIX:

#

export VERBOSE ?= 1

# program aliases
CMAKE ?= cmake
RM = $(CMAKE) -E rm -rf # force this one to use CMake
PYTHON ?= python3
DOCKER ?= docker

# directory aliases
BUILDS ?= Builds
DOCS ?= doc
DEPS_GRAPH ?= deps_graph

#

override LIMES_DOCS = $(patsubst %/,%,$(strip $(dir $(realpath $(firstword $(MAKEFILE_LIST))))))

LIMES_ROOT ?= $(LIMES_DOCS)/..

#

.PHONY: help
help:  ## Print this message
	@grep -E '^[a-zA-Z_-]+:.*?\#\# .*$$' $(LIMES_DOCS)/Makefile | sort | awk 'BEGIN {FS = ":.*?\#\# "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

#

.PHONY: docker
docker: ## Builds the Docker image
	$(DOCKER) build $(LIMES_DOCS) --tag limes_docs:latest

# init, cmake config, deps_graph, docs, clean, docker deploy
