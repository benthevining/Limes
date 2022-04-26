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
	DIRECTORY_NAME "harmony/scales"
	TARGET limes_music
	INSTALL_COMPONENT limes_music_dev
	INSTALL_DIR "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  Chromatic.cpp
		  Chromatic.h
		  KeySignature.cpp
		  KeySignature.h
		  KeySignature_impl.h
		  Mode.cpp
		  Mode.h
		  Octatonic.cpp
		  Octatonic.h
		  Scale.cpp
		  Scale.h
		  WholeTone.cpp
		  WholeTone.h)

set (scales_files ${harmony/scales_files} PARENT_SCOPE)
