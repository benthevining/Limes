#!/bin/sh

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

# inputs:
# GH_REPO_TOKEN

set -euo

readonly GH_REPO_REF="github.com/benthevining/Limes.git"

script_dir="$(cd "$(dirname "$0")" && pwd)"
readonly script_dir

readonly temp_dir="$script_dir/docs"

if [ -d "$temp_dir" ]; then
	rm -rf "$temp_dir"
fi

mkdir "$temp_dir"
cd "$temp_dir"

git clone -b docs --recurse-submodules "https://git@$GH_REPO_REF"

readonly docs_git_tree="$temp_dir/Limes"

readonly limes_root="$script_dir/.."

cd "$limes_root"

cmake --preset default -D LIMES_INTERNAL_DOCS=OFF

cmake --build --preset docs

cd "$docs_git_tree"

# remove everything currently in the docs branch
rm -rf -- *

# copy generated docs to cloned copy of docs git tree
mv "$limes_root"/doc/html/* "$docs_git_tree"

# need to create an empty .nojekyll file
touch .nojekyll

git config push.default simple
git config user.name "Github Actions"
git config user.email "actions@github.com"

git add --all .

git commit -am "Updating docs" --allow-empty

git push --force "https://${GH_REPO_TOKEN}@${GH_REPO_REF}" --no-verify

exit 0
