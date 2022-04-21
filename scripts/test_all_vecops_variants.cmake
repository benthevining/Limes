# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

set (LIMES_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
set (BINARY_DIR "${LIMES_ROOT}/Builds")

if(CMAKE_GENERATOR)
	set (generator_flag -G "'${CMAKE_GENERATOR}'")
endif()

#

function(remove_build_directory)
	file (REMOVE_RECURSE "${BINARY_DIR}")
endfunction()

function(execute_build)
	execute_process (
		COMMAND "${CMAKE_COMMAND}" --build --preset default
		WORKING_DIRECTORY "${LIMES_ROOT}" COMMAND_ECHO STDOUT COMMAND_ERROR_IS_FATAL ANY)
endfunction()

#

# vDSP

remove_build_directory ()

execute_process (COMMAND "${CMAKE_COMMAND}" -B Builds ${generator_flag}
				 WORKING_DIRECTORY "${LIMES_ROOT}" COMMAND_ECHO STDOUT COMMAND_ERROR_IS_FATAL ANY)

execute_build ()

# vDSP (no FFTW)

remove_build_directory ()

execute_process (COMMAND "${CMAKE_COMMAND}" -B Builds ${generator_flag} -D LIMES_IGNORE_FFTW=ON
				 WORKING_DIRECTORY "${LIMES_ROOT}" COMMAND_ECHO STDOUT COMMAND_ERROR_IS_FATAL ANY)

execute_build ()

#

# IPP

remove_build_directory ()

execute_process (COMMAND "${CMAKE_COMMAND}" -B Builds ${generator_flag} -D LIMES_USE_IPP=ON
				 WORKING_DIRECTORY "${LIMES_ROOT}" COMMAND_ECHO STDOUT COMMAND_ERROR_IS_FATAL ANY)

execute_build ()

# IPP (no FFTW)

remove_build_directory ()

execute_process (
	COMMAND "${CMAKE_COMMAND}" -B Builds ${generator_flag} -D LIMES_USE_IPP=ON -D
			LIMES_IGNORE_FFTW=ON WORKING_DIRECTORY "${LIMES_ROOT}" COMMAND_ECHO STDOUT
												   COMMAND_ERROR_IS_FATAL ANY)

execute_build ()

#

# MIPP

remove_build_directory ()

execute_process (COMMAND "${CMAKE_COMMAND}" -B Builds ${generator_flag} -D LIMES_USE_MIPP=ON
				 WORKING_DIRECTORY "${LIMES_ROOT}" COMMAND_ECHO STDOUT COMMAND_ERROR_IS_FATAL ANY)

execute_build ()

# MIPP (no FFTW)

remove_build_directory ()

execute_process (
	COMMAND "${CMAKE_COMMAND}" -B Builds ${generator_flag} -D LIMES_USE_MIPP=ON -D
			LIMES_IGNORE_FFTW=ON WORKING_DIRECTORY "${LIMES_ROOT}" COMMAND_ECHO STDOUT
												   COMMAND_ERROR_IS_FATAL ANY)

execute_build ()

#

# Fallback

remove_build_directory ()

execute_process (
	COMMAND "${CMAKE_COMMAND}" -B Builds ${generator_flag} -D LIMES_USE_VECOPS_FALLBACK=ON
	WORKING_DIRECTORY "${LIMES_ROOT}" COMMAND_ECHO STDOUT COMMAND_ERROR_IS_FATAL ANY)

execute_build ()

# Fallback (no Pommier)

remove_build_directory ()

execute_process (
	COMMAND "${CMAKE_COMMAND}" -B Builds ${generator_flag} -D LIMES_USE_VECOPS_FALLBACK=ON -D
			LIMES_IGNORE_POMMIER=ON WORKING_DIRECTORY "${LIMES_ROOT}" COMMAND_ECHO STDOUT
													  COMMAND_ERROR_IS_FATAL ANY)

execute_build ()

# Fallback (no FFTW)

remove_build_directory ()

execute_process (
	COMMAND "${CMAKE_COMMAND}" -B Builds ${generator_flag} -D LIMES_USE_VECOPS_FALLBACK=ON -D
			LIMES_IGNORE_FFTW=ON WORKING_DIRECTORY "${LIMES_ROOT}" COMMAND_ECHO STDOUT
												   COMMAND_ERROR_IS_FATAL ANY)

execute_build ()
