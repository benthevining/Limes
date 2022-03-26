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

if [ -z "${MAKEFILE_CACHE}" ]; then
	CACHE_DIR="$LIMES_ROOT/Cache"
else
	CACHE_DIR="$MAKEFILE_CACHE"
fi

CMAKE_SCRIPT="$CACHE_DIR/generate_makefile.cmake"
OUTPUT_FILE="$LIMES_ROOT/Makefile"

#

remove_cached_files()
{
	echo "Removing cached scripts..."
	rm -rf "$CMAKE_SCRIPT" "$CACHE_DIR/generate_makefile.cmake"
}

print_help()
{
	echo "Makefile generation script

Flags:
	-r|--remove : remove and refetch the cached scripts before generation
	-h|--help : print this message and exit

Arguments:
	-c|--cache : pass the directory to cache files in as an argument

Environment variables:
	MAKEFILE_CACHE : the directory to cache files in
"

	exit 0
}

for arg in "$@"; do
	case "${arg}" in
		-r) remove_cached_files;;
		--remove) remove_cached_files;;
		-h) print_help;;
		--help) print_help;;
	esac
done

while getopts c:cache: flag
do
	case "${flag}" in
		c) CACHE_DIR="${OPTARG}";;
		cache) CACHE_DIR="${OPTARG}";;
	esac
done

#

if ! [ -f "$CMAKE_SCRIPT" ]; then
	curl -o "$CMAKE_SCRIPT" "https://raw.githubusercontent.com/benthevining/Oranges/main/scripts/makefiles/generate_makefile.cmake"
fi

#

cmake -D PROJECT_NAME=LIMES -D "OUT_FILE=$OUTPUT_FILE" -P "$CMAKE_SCRIPT"

exit 0
