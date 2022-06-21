#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
A simple script to replace the Oranges version in various places where it is hard coded in Limes's CMake files.
"""

# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

from argparse import ArgumentParser
from pathlib import Path

#

parser = ArgumentParser()

parser.add_argument(
    "--old-version",
    action="store",
    dest="old_version",
    type=str,
    required=True,
    help=
    "The previous Oranges version string that can be found in the Limes CMake files"
)

parser.add_argument(
    "--new-version",
    action="store",
    dest="new_version",
    type=str,
    required=True,
    help="The new Oranges version string to insert in the Limes CMake files")

parser.add_argument("--limes-root",
                    action="store",
                    dest="limes_root",
                    type=Path,
                    required=False,
                    default=Path(__file__).parent.parent.absolute(),
                    help="Path to the root of the Limes repository")

args = parser.parse_args()

#

main_cmakelists = args.limes_root / "CMakeLists.txt"

with open(main_cmakelists, "r", encoding="UTF-8") as f:
	text = f.read()

text = text.replace(f"find_package (Oranges {args.old_version} REQUIRED)",
                    f"find_package (Oranges {args.new_version} REQUIRED)", 1)

with open(main_cmakelists, "w", encoding="UTF-8") as f:
	f.write(text)

#

programs_cmakelists = args.limes_root / "programs" / "CMakeLists.txt"

with open(programs_cmakelists, "r", encoding="UTF-8") as f:
	text = f.read()

text = text.replace(f"find_package (Oranges {args.old_version} REQUIRED)",
                    f"find_package (Oranges {args.new_version} REQUIRED)", 1)

with open(programs_cmakelists, "w", encoding="UTF-8") as f:
	f.write(text)

#

config_script = args.limes_root / "cmake" / "Limes-config.cmake"

with open(config_script, "r", encoding="UTF-8") as f:
	text = f.read()

text = text.replace(f"find_dependency (Oranges {args.old_version})",
                    f"find_dependency (Oranges {args.new_version})", 1)

with open(config_script, "w", encoding="UTF-8") as f:
	f.write(text)
