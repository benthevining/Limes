#!/bin/sh
# This file exports the LIMES_PATH environment variable.
# This variable allows any CMake project consuming Limes to find this directory locally, instead of fetching the sources from GitHub.
# I recommend that you source this script from your shell startup script.

# @formatter:off
# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================
# @formatter:on

# Public: The path to the Limes repository root.
# This environment variable is checked by the FindLimes.cmake module shipped with Limes,
# so if consuming CMake projects use this module to include Limes, then if this script has
# been sourced, this local directory will be found by the CMake configuration.
export LIMES_PATH="${LIMES_PATH:-$(cd "$(dirname "$0")" && pwd)}"

# Public: This function changes the current working directory to the Limes repository.
go_limes() {
	cd "$LIMES_PATH"
}
