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

#

remove_cached_files()
{
	echo "Removing cached scripts..."
	rm -rf "$CMAKE_SCRIPT" "$CACHE_DIR/generate_makefile.cmake"
}

for arg in "$@"; do
	case "${arg}" in
		-r) remove_cached_files;;
		--remove) remove_cached_files;;
	esac
done

#

if ! [ -f "$CMAKE_SCRIPT" ]; then
	curl -o "$CMAKE_SCRIPT" "https://raw.githubusercontent.com/benthevining/Oranges/main/scripts/makefiles/generate_makefile.cmake"
fi

#

cmake -D PROJECT_NAME=LIMES -D "OUT_FILE=$OUTPUT_FILE" -P "$CMAKE_SCRIPT"
