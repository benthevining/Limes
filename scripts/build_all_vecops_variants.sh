#!/bin/sh
# shellcheck disable=SC2181

# @formatter:off
# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================
# @formatter:on

set -u

export CMAKE_GENERATOR="Xcode"
export VERBOSE=1
export CMAKE_BUILD_PARALLEL_LEVEL=4

script_dir="$(cd "$(dirname "$0")" && pwd)"
readonly script_dir

readonly limes_root="$script_dir/.."

readonly logs_dir="$limes_root/logs"
readonly deploy_dir="$limes_root/deploy"

rm -rf "$logs_dir" "$deploy_dir"

mkdir -p "$logs_dir"

#

for define in LIMES_USE_VDSP LIMES_USE_IPP LIMES_USE_IPP LIMES_USE_MIPP LIMES_USE_VECOPS_FALLBACK
do
	echo "Vecops variant: $define"

	rm -rf "$limes_root/Builds" "$limes_root/tests/Builds"

	log_subdir="$logs_dir/$define"

	[ -d "$log_subdir" ] || mkdir -p "$log_subdir"

	config_log="$log_subdir/configure.log"

	cd "$limes_root" || exit 1

	if [ "$define" = "LIMES_USE_VECOPS_FALLBACK" ]; then
		cmake -B Builds -D LIMES_USE_VECOPS_FALLBACK=ON -D LIMES_IGNORE_POMMIER=ON -D LIMES_IGNORE_FFTW=ON -D LIMES_BUILD_DOCS=OFF -D LIMES_BUILD_TESTS=OFF | tee "$config_log"
	else
		cmake -B Builds -D "$define=ON" -D LIMES_IGNORE_FFTW=ON -D LIMES_BUILD_DOCS=OFF -D LIMES_BUILD_TESTS=OFF | tee "$config_log"
	fi

	if [ $? -eq 0 ]; then
		echo "CMake configure succeeded!"
	else
		echo "CMake configure failed - variant $define. Log is at $config_log"
		exit $?
	fi

	for config in Debug Release
	do
		build_log="$log_subdir/$config.log"

		echo "Build configuration: $config"

		cd "$limes_root" || exit 1

		cmake --build --preset maintainer --config "$config" | tee "$build_log"

		if [ $? -eq 0 ]; then
			echo "Build succeeded!"
		else
			echo "Build failed - config $config, vecops variant $define. Log is at $build_log"
			exit $?
		fi

		echo "Installing configuration $config..."

		install_log="$log_subdir/install_$config.log"

		cmake --install "$limes_root/Builds" --config "$config" --prefix "$deploy_dir/$define/$config" | tee "$install_log"

		if ! [ $? -eq 0 ]; then
			echo "Installing failed. Log is at $install_log"
			exit $?
		fi

		# TO DO: build tests against the compiled libs...
	done
done

#

echo "Everything succeeded!"

exit 0
