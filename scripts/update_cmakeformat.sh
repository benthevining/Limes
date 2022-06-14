#!/usr/bin/sh

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

script_dir="$(cd "$(dirname "$0")" && pwd)"
readonly script_dir

readonly output_file="$script_dir/../.cmake-format.json"

readonly python_script="$ORANGES_PATH/scripts/update_cmakeformat_config.py"

python3 "$python_script" \
	--input "$ORANGES_PATH/util/OrangesCMakeCommands.json" \
	--output "$output_file"

python3 "$python_script" \
	--input "$script_dir/../util/LimesCMakeCommands.json" \
	--output "$output_file"
