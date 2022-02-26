# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

target_sources (limes_music PRIVATE NoteDuration.h NoteDuration_impl.h NoteLength.h
									NoteLength_impl.h NoteLength.cpp TimeSignature.h)

target_include_directories (limes_core PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
