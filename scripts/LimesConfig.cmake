# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include_guard (GLOBAL)

cmake_minimum_required (VERSION 3.21 FATAL_ERROR)

include (CMakeFindDependencyMacro)

@PACKAGE_INIT@

# find_dependency ()

include ("@LIMES_INSTALL_DEST@/LimesTargets.cmake")

#

check_required_components ("@PROJECT_NAME@")
