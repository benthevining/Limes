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
	DIRECTORY_NAME util
	TARGET limes_audio
	INSTALL_COMPONENT limes_audio_dev
	INSTALL_DIR "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  CircularBuffer.cpp
		  CircularBuffer.h
		  Misc.h
		  MonoStereoConverter.cpp
		  MonoStereoConverter.h
		  PerlinNoise.cpp
		  PerlinNoise.h
		  PitchDetector.cpp
		  PitchDetector.h)

set (util_files ${util_files} PARENT_SCOPE)
