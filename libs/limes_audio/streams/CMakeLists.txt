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
	DIRECTORY_NAME streams
	TARGET limes_audio
	INSTALL_COMPONENT limes_audio_dev
	INSTALL_DIR "${LIMES_AUDIO_INSTALL_INCLUDE_DIR}"
	FILES # cmake-format: sortable
		  oscillator.cpp oscillator.h SampleStream.cpp SampleStream.h
		  WhiteNoiseGenerator.cpp WhiteNoiseGenerator.h)

set (streams_files ${streams_files} PARENT_SCOPE)
