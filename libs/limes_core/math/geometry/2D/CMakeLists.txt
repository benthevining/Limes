# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

include (OrangesSourceFileUtils)

oranges_add_source_files (
	DIRECTORY_NAME "math/geometry/2D"
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  angle.h
		  circle.h
		  rectangle.h
		  square.h
		  triangle.h
		  pentagon.h
		  hexagon.h
		  octagon.h
		  2Dshape.h)

set_source_files_properties (2Dshape.h TARGET_DIRECTORY limes_core PROPERTIES ABSTRACT ON)

set (2D_files ${math/geometry/2D_files} PARENT_SCOPE)
