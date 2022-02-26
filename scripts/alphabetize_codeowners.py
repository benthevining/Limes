#!/usr/bin/env python
# -*- coding: utf-8 -*-

# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

import os


def main():

	LEMONS_ROOT = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))

	CODEOWNERS = os.path.join(LEMONS_ROOT, '.github', 'CODEOWNERS')

	with open(CODEOWNERS, 'r') as f:
		file_lines = f.readlines()

	# remove empty lines
	file_lines = list(filter(None, file_lines))

	# sort remaining lines
	file_lines.sort()

	with open(CODEOWNERS, 'w') as f:
		f.truncate(0)
		f.write(''.join(file_lines))


if __name__ == '__main__':
	main()
