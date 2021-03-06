<!-- markdownlint-disable -->
```
 __    ____  __  __  ____  ___
(  )  (_  _)(  \/  )( ___)/ __)
 )(__  _)(_  )    (  )__) \__ \
(____)(____)(_/\/\_)(____)(___/
```

![GitHub top language](https://img.shields.io/github/languages/top/benthevining/Limes)
[![Create release](https://github.com/benthevining/Limes/actions/workflows/test_and_release.yml/badge.svg)](https://github.com/benthevining/Limes/actions/workflows/test_and_release.yml)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/benthevining/Limes/main.svg)](https://results.pre-commit.ci/latest/github/benthevining/Limes/main)
[![Build docs](https://github.com/benthevining/Limes/actions/workflows/docs.yml/badge.svg)](https://github.com/benthevining/Limes/actions/workflows/docs.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/benthevining/limes/badge)](https://www.codefactor.io/repository/github/benthevining/limes)
[![CodeQL](https://github.com/benthevining/Limes/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/benthevining/Limes/actions/workflows/codeql-analysis.yml)
[![semantic-release: conventionalcommits](https://img.shields.io/badge/semantic--release-conventionalcommits-e10079?logo=semantic-release)](https://github.com/semantic-release/semantic-release)
![License](https://img.shields.io/github/license/benthevining/Limes)
![GitHub repo size](https://img.shields.io/github/repo-size/benthevining/Limes)
![Lines of code](https://img.shields.io/tokei/lines/github/benthevining/Limes)
![GitHub commit activity](https://img.shields.io/github/commit-activity/m/benthevining/Limes)
![GitHub Release Date](https://img.shields.io/github/release-date/benthevining/Limes)
![GitHub release](https://img.shields.io/github/v/release/benthevining/Limes)
![GitHub Sponsors](https://img.shields.io/github/sponsors/benthevining?style=social)
![GitHub Repo stars](https://img.shields.io/github/stars/benthevining/Limes?style=social)
![GitHub followers](https://img.shields.io/github/followers/benthevining?style=social)

A collection of standalone C++ static libraries.

## What's here

Limes consists of the following libraries:

* [limes_core](libs/limes_core/README.md), the core library of utilities
* [limes_audio](libs/limes_audio/README.md), for realtime audio and DSP
* [limes_midi](libs/limes_midi/README.md), for basic MIDI support utilities
* [limes_music](libs/limes_music/README.md), for music-theory related concepts
* [limes_vecops](libs/limes_vecops/README.md), for SIMD-accelerated vector operations

Limes also provides the following command line tools:
* [BinaryBuilder](programs/binary_builder/README.md), a utility for embedding binary resource files into C++ source code
* [FileUtil](programs/file_util/README.md), a utility to provide some universal filesystem operations

## Using Limes

You can run `cmake --install` and then call `find_package (Limes)` from your consuming project, and everything should just work out of the box.

If your project depends on Limes, I recommend copying the `FindLimes` script from the `scripts/` directory into your project's source tree (and adding its location to the `CMAKE_MODULE_PATH` before calling `find_package (Limes)`), so that if your project is built on a system where Limes hasn't been installed, it can still be fetched at configure-time.
See the `FindLimes` file for more documentation on what it does.

## Compiling Limes

Limes makes use of C++20, notably concepts. Limes has been tested on Mac, Windows, and Linux with Clang, GCC, and MSVC. Limes is also tested crosscompiled for iOS, tvOS and watchOS.

To build the Limes unit tests, pass `-D LIMES_BUILD_TESTS=ON` to the CMake configuration. This option defaults to `OFF`.


## CMake information

### Options

The vector operations library has several options affecting its configuration and behavior; see [its readme](libs/limes_vecops/README.md) for details.

* `LIMES_BUILD_PROGRAMS`

Builds the command-line utility programs that Limes ships. This option defaults to `ON`.

* `LIMES_BUILD_TESTS`

Builds the Limes unit tests. Defaults to OFF if Limes is not the top-level directory CMake is invoked in.

* `LIMES_BUILD_DOCS`

Builds the Limes documentation. Defaults to OFF if Limes is not the top-level directory CMake is invoked in.

* `LIMES_BUILD_JUCE`

Builds the JUCE bindings for the Limes libraries ((JUCE modules providing JUCE API wrappers for Limes classes).
Setting this to `OFF` will build only the libraries and programs.
This option defaults to `ON`.

* `LIMES_BUILD_EXAMPLE_PLUGINS`

Builds the example JUCE plugins. This option requires that `LIMES_BUILD_JUCE` is `ON`.

* `LIMES_MINIMAL`

Builds only the core C++ libraries and nothing else.


### Dependencies

Limes's CMake configuration depends on [Oranges](https://github.com/benthevining/Oranges).
You can set the environment or command-line variables `ORANGES_PATH` to point Limes to a local copy; if it is already installed on your system, it should be found automatically.
Otherwise, the Oranges source code will be downloaded from GitHub at configure time, using CMake's FetchContent module.

### Targets

Limes creates the following CMake targets:

* Limes::limes_core
* Limes::limes_vecops
* Limes::limes_music
* Limes::limes_audio
* Limes::limes_midi
* Limes::BinaryBuilder

All of these are static library targets. Additionally, there is a `Limes::Limes` interface library target that links to all of the library targets (but not the BinaryBuilder).

### Install components

Limes creates the following CPack install components:

* limes_vecops_dev
* limes_vecops_runtime
* limes_music_dev
* limes_music_runtime
* limes_core_dev
* limes_core_runtime
* limes_audio_dev
* limes_audio_runtime
* limes_midi_dev
* limes_midi_runtime
* limes_libs_dev
* limes_libs_runtime
* limes_binary_builder_dev
* limes_binary_builder_runtime
* limes_programs_dev
* limes_programs_runtime
* limes_dev (aggregate of all other dev components)
* limes_runtime (aggregate of all other runtime components)
* limes_doc_pdf
* limes_doc_html
* limes_doc_man
* limes_docs

Limes also creates the following CPack install component groups:

* limes_vecops
* limes_music
* limes_core
* limes_audio
* limes_midi
* limes_libs
* limes_binary_builder
* limes_programs
* limes_sphinx_docs
* limes_doxy_docs
* limes_docs
* limes (aggregate of all Limes components)

### find_package components

When you call `find_package (Limes)`, you can pass the following component names:

* All
* Libs
* Programs
* Audio
* MIDI
* Core
* Music
* Vecops
* BinaryBuilder
* FileUtil

If no component names are specified, Limes will default to importing everything.

## Dependency graph

<!-- editorconfig-checker-disable -->
<p align="center">
	<img src="https://github.com/benthevining/Limes/blob/main/util/deps_graph.png" alt="Limes dependency graph"/>
</p>
<!-- editorconfig-checker-enable -->
