#!/bin/sh

# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
LIMES_ROOT="$SCRIPT_DIR/.."
CACHE_DIR="$LIMES_ROOT/Cache"

CMAKE_SCRIPT="$CACHE_DIR/generate_makefile.cmake"
OUTPUT_FILE="$LIMES_ROOT/Makefile"

if ! [ -f "$CMAKE_SCRIPT" ]; then
	curl -o "$CMAKE_SCRIPT" "https://raw.githubusercontent.com/benthevining/Oranges/main/scripts/makefiles/generate_makefile.cmake"
fi

if [ -f "$OUTPUT_FILE" ]; then
	rm -rf "$OUTPUT_FILE"
fi

cmake -D PROJECT_NAME=LIMES -D "OUT_FILE=$OUTPUT_FILE" -P "$CMAKE_SCRIPT"
