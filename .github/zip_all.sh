#!/bin/sh

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

set -euo

script_dir="$(cd "$(dirname "$0")" && pwd)"
readonly script_dir

readonly limes_root="$script_dir/.."

for id_key in WindowsClang WindowsMSVC LinuxClang LinuxGCC MacGCC MacXcode iOS tvOS watchOS
do
	cmake -E copy_directory "$limes_root/stage/$id_key.debug" "$limes_root/stage/$id_key.release" "$limes_root/deploy/$id_key"
	cmake -E tar cvz "$id_key.zip" "$limes_root/deploy/$id_key"
done

rm -rf "$limes_root/stage"

exit 0
