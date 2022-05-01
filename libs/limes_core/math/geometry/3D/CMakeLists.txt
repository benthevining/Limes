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
	DIRECTORY_NAME "math/geometry/3D"
	TARGET limes_core
	INSTALL_COMPONENT limes_core_dev
	INSTALL_DIR "${LIMES_CORE_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  3Dshape.h
		  cone.h
		  cube.h
		  cylinder.h
		  pyramid.h
		  rectangular_prism.h
		  sphere.h
		  triangular_prism.h)

set_source_files_properties (3Dshape.h TARGET_DIRECTORY limes_core
							 PROPERTIES ABSTRACT ON)

set (3D_files ${math/geometry/3D_files} PARENT_SCOPE)
