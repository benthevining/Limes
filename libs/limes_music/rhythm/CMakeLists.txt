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
	DIRECTORY_NAME rhythm
	TARGET limes_music
	INSTALL_COMPONENT limes_music_dev
	INSTALL_DIR "${LIMES_MUSIC_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  NoteDuration.h NoteDuration_impl.h NoteLength.cpp NoteLength.h NoteLength_impl.h
		  TimeSignature.h)

set (rhythm_files ${rhythm_files} PARENT_SCOPE)
