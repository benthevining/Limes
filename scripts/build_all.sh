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


# Build every configuration of every CMake preset in the root directory

for preset in maintainer ninja-gcc iOS tvOS watchOS fallbacks
do
	echo "CMake preset: $preset"

	rm -rf "$limes_root/Builds" "$limes_root/tests/Builds"

	log_subdir="$logs_dir/$preset"

	[ -d "$log_subdir" ] || mkdir -p "$log_subdir"

	config_log="$log_subdir/configure.log"

	cd "$limes_root" || exit 1

	cmake --preset "$preset" | tee "$config_log"

	if [ $? -eq 0 ]; then
		echo "CMake configure succeeded!"
	else
		echo "CMake configure failed - preset $preset. Log is at $config_log"
		exit $?
	fi

	for config in Debug Release MinSizeRel RelWithDebInfo
	do
		if [ "$preset" = "ninja-gcc" ] && [ "$config" = "MinSizeRel" ]; then
			continue
		fi

		build_log="$log_subdir/$config.log"

		echo "Build configuration: $config"

		cd "$limes_root" || exit 1

		cmake --build --preset maintainer --config "$config" | tee "$build_log"

		if [ $? -eq 0 ]; then
			echo "Build succeeded!"
		else
			echo "Build failed - config $config, CMake preset $preset. Log is at $build_log"
			exit $?
		fi

		echo "Installing configuration $config..."

		install_log="$log_subdir/install_$config.log"

		cmake --install "$limes_root/Builds" --config "$config" --prefix "$deploy_dir/$preset/$config" | tee "$install_log"

		if ! [ $? -eq 0 ]; then
			echo "Installing failed. Log is at $install_log"
			exit $?
		fi
	done

	if [ "$preset" = "iOS" ] || [ "$preset" = "tvOS" ] || [ "$preset" = "watchOS" ]; then
		continue
	fi

	# Build from the test dir

	echo "Building tests against the compiled libs..."

	cd "$limes_root/tests" || exit 1

	test_config_log="$log_subdir/tests_configure.log"

	cmake --preset "$preset" -D CMAKE_PREFIX_PATH="$deploy_dir/$preset/Debug;$deploy_dir/$preset/Release" | tee "$test_config_log"

	if [ $? -eq 0 ]; then
		echo "CMake configure succeeded!"
	else
		echo "CMake configure failed - preset $preset. Log is at $test_config_log"
		exit $?
	fi

	for config in Debug Release
	do
		echo "Building tests - configuration $config"

		test_build_log="$log_subdir/tests_build.log"

		cd "$limes_root/tests" || exit 1

		cmake --build --preset default --config "$config" | tee "$test_build_log"

		if [ $? -eq 0 ]; then
			echo "Build succeeded!"
		else
			echo "Build failed - config $config, CMake preset $preset. Log is at $test_build_log"
			exit $?
		fi

		cd "$limes_root/tests" || exit 1

		test_run_log="$log_subdir/tests_run.log"

		ctest --preset default -C "$config" | tee "$test_run_log"

		if [ $? -eq 0 ]; then
			echo "Tests succeeded!"
		else
			echo "Tests failed - config $config, CMake preset $preset. Log is at $test_run_log"
			exit $?
		fi
	done
done

#

echo "Everything succeeded!"

exit 0
