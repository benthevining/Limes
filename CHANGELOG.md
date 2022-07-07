# Limes changelog

## [5.0.3](https://github.com/benthevining/Limes/compare/v5.0.2...v5.0.3) (2022-07-07)


### Bug Fixes

* added -flax-vector-conversions to vecops with GCC ([0eb5aa7](https://github.com/benthevining/Limes/commit/0eb5aa7dc5a81ba946d9afb0e0b42334dd3e81fe))
* fixing const-correctness in SSE mathfun ([1c5381a](https://github.com/benthevining/Limes/commit/1c5381a11ad919360aa25790b60fef6679b51d85))

## [5.0.2](https://github.com/benthevining/Limes/compare/v5.0.1...v5.0.2) (2022-07-06)


### Bug Fixes

* fixed Windows path conversion issues ([a3ec0b7](https://github.com/benthevining/Limes/commit/a3ec0b7b7f13a371c7e84c8dda7ee204d6b37e40))

## [5.0.1](https://github.com/benthevining/Limes/compare/v5.0.0...v5.0.1) (2022-07-05)


### Bug Fixes

* fixing errors when not using FFTW ([2e3d7ad](https://github.com/benthevining/Limes/commit/2e3d7ad24835573b7adeadad0d0104370b81f74c))
* fixing path conversion issues in MSVC ([32125ad](https://github.com/benthevining/Limes/commit/32125adbc1b0293279afb643dcce0792bc9fdd71))

## [5.0.0](https://github.com/benthevining/Limes/compare/v4.8.1...v5.0.0) (2022-07-03)


### ⚠ BREAKING CHANGES

* The Google Protobuf compiler and library are now required dependencies.

Precommit-Verified: 484b433a6aecfa2ec12fc45b4f13d9601f5c004f1a50ad913660e496c61170e5

### Features

* added a command-line argument parser class ([5f405ad](https://github.com/benthevining/Limes/commit/5f405ad8e732de2399c31d8b8945d88ed854e5da))
* added a func to check if running under debugger; refactored assertion macros ([a9950f3](https://github.com/benthevining/Limes/commit/a9950f33a3ccb5a8aa397f2d42d1e62b74406752))
* added a RawData class ([f0daf21](https://github.com/benthevining/Limes/commit/f0daf21c27482f0de14660dcc2eaf6770f6ab075))
* added a SampleProcessor class ([5319b67](https://github.com/benthevining/Limes/commit/5319b6737e61c4f929f97dfe96cef3aab3088103))
* added a stats accumulator and functions for converting numbers to roman numeral strings ([7d6d224](https://github.com/benthevining/Limes/commit/7d6d2243fc50d200375a4e835ac2d540ec460bca))
* added a TypeList class ([bf0f702](https://github.com/benthevining/Limes/commit/bf0f702111d0ee74a37c62b8f403d51049295d01))
* added an ArrayPointer class ([73335f3](https://github.com/benthevining/Limes/commit/73335f374686628cd38dd5f95fc0fd0d4c07841b))
* added an AudioBuffer class. BREAKING CHANGE: Removed the limes_data_structures and limes_locale libraries; these classes have been moved into limes_core. ([064915b](https://github.com/benthevining/Limes/commit/064915bbc9d41af8470a2350833bc76c79d7e779))
* added functions to get the path to the current executable ([102b857](https://github.com/benthevining/Limes/commit/102b857c64ac2a37c66965cd22f8d244e3619689))
* added hashing functions ([7de6dab](https://github.com/benthevining/Limes/commit/7de6dab2a26e21eaef0f0be88515e50a9bfca901))
* added macros to disable/enable all compiler warnings ([19a6d39](https://github.com/benthevining/Limes/commit/19a6d394e95c2f6132239d38b25694a0bf0790db))
* added more FileUtil features ([f4d2a25](https://github.com/benthevining/Limes/commit/f4d2a256cc9dd39fe7514cf61a2fb93c10e9e4d2))
* added more FileUtil features ([5b3706a](https://github.com/benthevining/Limes/commit/5b3706a0133fcd938818621de98feffbb3e0520c))
* added more fileutil funcs ([0cc9957](https://github.com/benthevining/Limes/commit/0cc995747970eb3805102450df4934e48d81043f))
* added more fileutil funcs ([bd144ab](https://github.com/benthevining/Limes/commit/bd144abadc5e86c5d2f067152b81f6ef8f27b063))
* added more TypeList features ([11a49a1](https://github.com/benthevining/Limes/commit/11a49a14efff619cd242131236a43f18ce76321a))
* added more vecops funcs ([d33df8f](https://github.com/benthevining/Limes/commit/d33df8fbe2bdc8ba19f8d63b27ff028049fca8e6))
* added more vecops functions ([8acde2e](https://github.com/benthevining/Limes/commit/8acde2e1a8f40bd99653fc4b5444db8f9b10d245))
* added Rakefile ([aead175](https://github.com/benthevining/Limes/commit/aead175c6d7e082332ac9df39fbe5842bd56fdf9))
* added specializations of std::hash ([ecd8d67](https://github.com/benthevining/Limes/commit/ecd8d670c542678d742c54621213c07c0d270dfd))
* added utility functions for converting between units of memory ([4cf0b80](https://github.com/benthevining/Limes/commit/4cf0b8018e0e3174697b15fb45473b7f0770a3f5))
* expanding time library ([bc42723](https://github.com/benthevining/Limes/commit/bc42723f3a187db54a55546a35988ba230e94403))
* initial commit of a JSON parser ([5eabf40](https://github.com/benthevining/Limes/commit/5eabf40935e2c24e906285389c6175ed746ac735))
* initial commit of a JSON parser ([959b568](https://github.com/benthevining/Limes/commit/959b568a27d5a7fd824dca38f99a1aa1c7c5e372))
* initial commit of fileutil functionality ([5bf42b7](https://github.com/benthevining/Limes/commit/5bf42b74ca275e23fdaae21b3442ce31c64ed802))
* integrated RawData with binary builder ([b327136](https://github.com/benthevining/Limes/commit/b3271368ec587f098c52240f3f2ead74c68674ef))
* made hash algos' implementations public as classes ([eaa3a91](https://github.com/benthevining/Limes/commit/eaa3a91acc284c9648463b4ff3e2a31670f65a61))
* more Typelist functions ([7daad32](https://github.com/benthevining/Limes/commit/7daad32cd895d32d2db6bdc99e8ed458c89108ff))
* more TypeList functions ([8c8649b](https://github.com/benthevining/Limes/commit/8c8649bb1fb1a90a1f35cfdb1c0acdb645746fa8))
* updating ArgumentParser class to handle positional arguments ([cf61679](https://github.com/benthevining/Limes/commit/cf61679cf56e716f646806a7021ffa1f2ba8eafa))


### Bug Fixes

* **deps:** update dependency conventional-changelog-conventionalcommits to v5 ([5860889](https://github.com/benthevining/Limes/commit/586088927e8f9a0c016a4a4bc146270589a669cd))
* **deps:** update dependency semantic-release to v19.0.3 ([42fff79](https://github.com/benthevining/Limes/commit/42fff796370a9cb083f2bd5e69e76dfe329cb473))
* fixed a bunch of implicit conversion warnings ([d77e806](https://github.com/benthevining/Limes/commit/d77e80671f3d9fd9c946edbe5b230d728837920e))
* fixed FileUtil compilation on Windows ([beca9e9](https://github.com/benthevining/Limes/commit/beca9e91c729d25fac5524a8472cb59c8abecf33))
* fixed output of dir separator char on windows ([dc2313a](https://github.com/benthevining/Limes/commit/dc2313adc0e1af2b5d9c477df90f2a8d8d9d943e))
* fixes for MSVC ([b7657f7](https://github.com/benthevining/Limes/commit/b7657f7b5f1504e682bb98c14b19060990c244f9))
* fixes with executable path funcs ([944ef89](https://github.com/benthevining/Limes/commit/944ef89806c7130ec04951667c2152dee6626558))
* fixing \'Microsoft cast\' error on Windows/Clang ([b580ae3](https://github.com/benthevining/Limes/commit/b580ae38f064c338ebabc06a6808352063292e2c))
* fixing Clock concepts for Xcode ([d624cf1](https://github.com/benthevining/Limes/commit/d624cf16211f16957bee6342a085dd7d18d23ecd))
* fixing dynamic library impl on Windows ([d1fa74f](https://github.com/benthevining/Limes/commit/d1fa74f9d256b0d14365cef51f3735477f54c2ef))
* fixing JSON parser; removed protobuf dependency ([8e93413](https://github.com/benthevining/Limes/commit/8e934134a1529e1d3865f582894f30b3161b9950))
* fixing Matrix::getSubmatrix() for GCC ([79a88ac](https://github.com/benthevining/Limes/commit/79a88acee95a2ed0957cd9d5f1e086967efefcdc))
* fixing MIPP errors ([9b73381](https://github.com/benthevining/Limes/commit/9b733812aff2f13f70af57bd5293c7145cc8bf8b))
* fixing windows DynamicLibrary ([5922c84](https://github.com/benthevining/Limes/commit/5922c84efb6b8557361bd0b73e00d44253056f98))
* making sure docs/doc/ directory gets committed ([6af5de7](https://github.com/benthevining/Limes/commit/6af5de7e9b71e08294132b35bfdf55a2e3d7b913))
* making things constexpr instead of constinit to please MSVC ([fc15421](https://github.com/benthevining/Limes/commit/fc154218ff7b576695b273b49014990dcfc2b691))
* misc fixes ([ad2ca3e](https://github.com/benthevining/Limes/commit/ad2ca3ebef3428c56f934757ac353de96f6a3bbe))
* more exec path func fixes ([92aa75e](https://github.com/benthevining/Limes/commit/92aa75e21fc74ef0a057bfabda1a2fbf2e8e3378))
* MSVC fixes ([89480b4](https://github.com/benthevining/Limes/commit/89480b4b97ae66a7e23d98ab2ae62ce666ded46a))
* removed ScopedLoopingThread class ([499a33b](https://github.com/benthevining/Limes/commit/499a33b20e238a1184b9f04da4f80ec7095c5cda))
* removing find_package(Protobuf) call from CMake) ([8d0dce3](https://github.com/benthevining/Limes/commit/8d0dce3bd422868cb34c3b57902c8ceec6c826f8))
* typo in DynamicLibrary ([52b0e4e](https://github.com/benthevining/Limes/commit/52b0e4eb3f8b030e35eb158cc878f4edbd118516))
* typo in TypeTraits.h ([fcfe679](https://github.com/benthevining/Limes/commit/fcfe679668686143b78a743ecdfb533f3e91022c))


* added Protobuf code generation. ([4a775a2](https://github.com/benthevining/Limes/commit/4a775a2cb0dca886c62b80275ce33b4e23a6f130))

### [4.8.1](http://github.com/benthevining/Limes/compare/v4.8.0...v4.8.1) (2022-05-02)


### Bug Fixes

* moving non-constexpr time funcs into .cpp file ([faf67d5](http://github.com/benthevining/Limes/commit/faf67d5004eb90c3726c20e841dcd55efd43eb6c))
* resolved multiple definitions issue in Date class ([faf3711](http://github.com/benthevining/Limes/commit/faf371192edd4621ab30f93ce3b9ddb701f8cfb4))

## [4.8.0](http://github.com/benthevining/Limes/compare/v4.7.1...v4.8.0) (2022-05-01)


### Features

* added geometry classes ([28635f6](http://github.com/benthevining/Limes/commit/28635f6925f21919272482abb43d17885d469e38))
* added more geometry classes, reorganizing ([0e5d24f](http://github.com/benthevining/Limes/commit/0e5d24f7492bcb901800dda27721a8637eba6120))

### [4.7.1](http://github.com/benthevining/Limes/compare/v4.7.0...v4.7.1) (2022-05-01)


### Bug Fixes

* fixed watchOS assembly issue ([b17b887](http://github.com/benthevining/Limes/commit/b17b88745649fae78a9ef92cee095767fe9ca9b6))

## [4.7.0](http://github.com/benthevining/Limes/compare/v4.6.0...v4.7.0) (2022-04-27)


### Features

* added some time helper classes ([aee485a](http://github.com/benthevining/Limes/commit/aee485a17a34e6fa67eaf6b7035f2ddfc538a8a5))
* including prebuilt binaries in releases ([d6f541b](http://github.com/benthevining/Limes/commit/d6f541ba4dac39e834271d6fc42d25961e6dfc80))


### Bug Fixes

* not using std::chrono::year class bc it's missing on Linux (and Mac GCC) ([4042215](http://github.com/benthevining/Limes/commit/404221531e69e513979eeea26588e4f066768351))

## [4.6.0](http://github.com/benthevining/Limes/compare/v4.5.0...v4.6.0) (2022-04-26)


### Features

* added a BinaryBuilder tool ([7d2484a](http://github.com/benthevining/Limes/commit/7d2484a13dc8db3d2c2060ad8575160358503aef))
* added a filesystem library wrapping std::filesystem ([1c1025d](http://github.com/benthevining/Limes/commit/1c1025d7b593e2042d7fc82ceb16ee35fd14bdb4))


### Bug Fixes

* cmake missing includes ([a669b8b](http://github.com/benthevining/Limes/commit/a669b8b09862ff3279033e585085e64ca7930e31))
* IPO bug with clang ([dfba652](http://github.com/benthevining/Limes/commit/dfba65254c8357d1fd414c905f57b3e209a3d36f))
* type safety in BinaryBuilder ([c34cdb0](http://github.com/benthevining/Limes/commit/c34cdb0fbf17c8757e282e73e304d4aa3a36d028))
* when cross compiling, building BinaryBuilder for host system, not target system ([11084e0](http://github.com/benthevining/Limes/commit/11084e0c082aa63b5ada9c6df22420f00f97c643))

## [4.5.0](http://github.com/benthevining/Limes/compare/v4.4.0...v4.5.0) (2022-04-22)


### Features

* added stack allocator, optional object classes ([82374a6](http://github.com/benthevining/Limes/commit/82374a6918525d1320107032ece547cb11453841))

## [4.4.0](http://github.com/benthevining/Limes/compare/v4.3.0...v4.4.0) (2022-04-21)


### Features

* added a basic arena-style allocator ([0ac4b21](http://github.com/benthevining/Limes/commit/0ac4b216859d262b2eeb70164a643ea96d8a163f))


### Bug Fixes

* const correctness bug in memory pool allocator ([4bbe2d2](http://github.com/benthevining/Limes/commit/4bbe2d224874684256c1babc699bc62dd8358b72))

## [4.3.0](http://github.com/benthevining/Limes/compare/v4.2.0...v4.3.0) (2022-04-21)


### Features

* added aligned allocation helpers ([f232cb2](http://github.com/benthevining/Limes/commit/f232cb29f9542d1178c269d9421e1da5a555e699))

## [4.2.0](http://github.com/benthevining/Limes/compare/v4.1.0...v4.2.0) (2022-04-21)


### Features

* added utilities for getting info about the build time & date, and some geometry functions ([3b766fb](http://github.com/benthevining/Limes/commit/3b766fb42b7c190200883afa6b5dc7601a3d3ae4))


### Bug Fixes

* added missing floating point flags for GCC ([4562829](http://github.com/benthevining/Limes/commit/456282952c2394456344c1b64da2c334a8489908))
* consteval bug in build date functions ([6bda524](http://github.com/benthevining/Limes/commit/6bda524316429ad15ce8a7707e3b9eacfc8fe813))
* GCC floating point flags for vecops ([4802ef4](http://github.com/benthevining/Limes/commit/4802ef4ec8e8e864f016ecd0cc9fba700fcc27ad))

## [4.1.0](http://github.com/benthevining/Limes/compare/v4.0.0...v4.1.0) (2022-04-19)


### Features

* added CMake presets for crosscompiling to tvOS, watchOS ([1e58fce](http://github.com/benthevining/Limes/commit/1e58fcef32b100323ef91a615812d5bc3ce20b76))

## [4.0.0](http://github.com/benthevining/Limes/compare/v3.12.0...v4.0.0) (2022-04-19)


### ⚠ BREAKING CHANGES

* all code is now within a versioned inline namespace, defined by the LIMES_BEGIN_NAMESPACE macro. This is to prevent ABI breakage in the future.

Precommit-Verified: f85584fee4fdc3614452498cf157680ad0926659570a1c6a140d8e351148a19a

* placing all code within an inline namespace for ABI protection ([1d26f61](http://github.com/benthevining/Limes/commit/1d26f617889bca232ea4a78a994f1c7a90fcd2b4))

## [3.12.0](http://github.com/benthevining/Limes/compare/v3.11.0...v3.12.0) (2022-04-18)


### Features

* added white noise & Perlin noise generator classes ([fc19d5f](http://github.com/benthevining/Limes/commit/fc19d5f7720d8e18281d4eb8650951da8da3bb19))

## [3.11.0](http://github.com/benthevining/Limes/compare/v3.10.0...v3.11.0) (2022-04-18)


### Features

* added a random number generator class ([9d3bde4](http://github.com/benthevining/Limes/commit/9d3bde43e6e83ddc8b5e2c73c372ad2e2e0d90f7))

## [3.10.0](http://github.com/benthevining/Limes/compare/v3.9.0...v3.10.0) (2022-04-18)


### Features

* moved PSOLA classes into Limes ([6447506](http://github.com/benthevining/Limes/commit/64475064c0734b6d6ec0b5ed0729079fbae90672))


### Bug Fixes

* fixing MIPP compiler errors ([6459d9f](http://github.com/benthevining/Limes/commit/6459d9f7efaad73e06dc6dc7745112a6256f0626))

## [3.9.0](http://github.com/benthevining/Limes/compare/v3.8.0...v3.9.0) (2022-04-18)


### Features

* added a SharedObject class ([5b02949](http://github.com/benthevining/Limes/commit/5b0294993e1ff1be39499f2a3f8fe0c030c5a981))

## [3.8.0](http://github.com/benthevining/Limes/compare/v3.7.0...v3.8.0) (2022-04-17)


### Features

* added a ReferenceCountedObject class ([6a1d8c5](http://github.com/benthevining/Limes/commit/6a1d8c51b13980878bb24c62f35b96fdea127d0b))

## [3.7.0](http://github.com/benthevining/Limes/compare/v3.6.0...v3.7.0) (2022-04-17)


### Features

* added a generic FIFO class ([51c4595](http://github.com/benthevining/Limes/commit/51c45955cb84c6eeeda611519b06e3aa2ca9b9af))

## [3.6.0](http://github.com/benthevining/Limes/compare/v3.5.0...v3.6.0) (2022-04-17)


### Features

* added a DetunableModel for oscillators ([44b9fed](http://github.com/benthevining/Limes/commit/44b9fede0ff2ef15a3501e8075ed1a0b66f3eafc))

## [3.5.0](http://github.com/benthevining/Limes/compare/v3.4.0...v3.5.0) (2022-04-16)


### Features

* added AbstractFIFO and CircularBuffer classes ([8a7778b](http://github.com/benthevining/Limes/commit/8a7778b675367becf0a3e69a70b2c1a6eec33b2c))
* added some MIDI helper classes ([f0720c2](http://github.com/benthevining/Limes/commit/f0720c2b8c35a90fe057001f21283745c0ff6cca))


### Bug Fixes

* updating package.json ([e3a5020](http://github.com/benthevining/Limes/commit/e3a5020c42625071167acff7c1a1062aa6af2b9a))

## [3.4.0](http://github.com/benthevining/Limes/compare/v3.3.1...v3.4.0) (2022-04-16)


### Features

* added more threading util funcs ([d35e6f9](http://github.com/benthevining/Limes/commit/d35e6f9404f7a88c7473ebeeb3318b867476dd50))


### Bug Fixes

* typo in sched_getaffinity call ([34e0b24](http://github.com/benthevining/Limes/commit/34e0b247d1e06670659bd9922176a4ff02ce2266))

### [3.3.1](http://github.com/benthevining/Limes/compare/v3.3.0...v3.3.1) (2022-04-16)


### Bug Fixes

* added a timeout to semantic release step ([e943eac](http://github.com/benthevining/Limes/commit/e943eace245ef461c1c1f9b72673067a34c93cd9))

## [3.3.0](http://github.com/benthevining/Limes/compare/v3.2.1...v3.3.0) (2022-04-15)


### Features

* including binaries in releases ([4a58ac4](http://github.com/benthevining/Limes/commit/4a58ac4fa06b169ba8c4add624faa8868d9b37bf))

### [3.2.1](http://github.com/benthevining/Limes/compare/v3.2.0...v3.2.1) (2022-04-15)


### Bug Fixes

* checking for MSVC or Windows before using aligned_alloc ([d48a0f4](http://github.com/benthevining/Limes/commit/d48a0f4b7545e4cfc70c032af739d46f5895502d))

## [3.2.0](http://github.com/benthevining/Limes/compare/v3.1.1...v3.2.0) (2022-04-13)


### Features

* added threading util funcs ([99a24de](http://github.com/benthevining/Limes/commit/99a24dead75e1044c4c7aa37b46ef247efbadb26))

### [3.1.1](http://github.com/benthevining/Limes/compare/v3.1.0...v3.1.1) (2022-04-13)


### Bug Fixes

* aligned alloc symbol for MSVC ([47c99eb](http://github.com/benthevining/Limes/commit/47c99eb8daa46c332fb411588c2418add919fc8b))

## [3.1.0](http://github.com/benthevining/Limes/compare/v3.0.1...v3.1.0) (2022-04-13)


### Features

* added an IgnoreUnused function ([53adff4](http://github.com/benthevining/Limes/commit/53adff475d69e49c7d645fbff5b6d775d5da6e96))

### [3.0.1](http://github.com/benthevining/Limes/compare/v3.0.0...v3.0.1) (2022-04-12)


### Bug Fixes

* regex bug in FFT impl selection ([318f377](http://github.com/benthevining/Limes/commit/318f377915ab6148a58da20d10aadd7c963c6b39))

## [3.0.0](http://github.com/benthevining/Limes/compare/v2.7.0...v3.0.0) (2022-04-12)


### ⚠ BREAKING CHANGES

* limes_core no longer depends on vecops; added new data structures and locale libs

Precommit-Verified: bc90a2efb74cae7a1ba76094d4efd00b84fdb174d8c0ecb20f4683c3cc6807ae

### Features

* added cartesianToPolar function ([d3a3c9f](http://github.com/benthevining/Limes/commit/d3a3c9f261119cde333ef91853aa765e07f48ed9))
* added more vecops funcs ([ebec5f1](http://github.com/benthevining/Limes/commit/ebec5f1be8981958e6d1d42d3bfa66a4fd3c15c4))
* integrated the pommier NEON and SSE implementations of sincos for vecops ([21bd43a](http://github.com/benthevining/Limes/commit/21bd43aa64e979436fbd514dd3d2c941e6faf430))


### Bug Fixes

* fixed syntax error in floating point status register funcs ([8d2694c](http://github.com/benthevining/Limes/commit/8d2694c14e7cf84bffdd91e989eadf831c657db3))
* fixing includes for SSE mathfun ([a486d5c](http://github.com/benthevining/Limes/commit/a486d5c9d2b13dfa7778fb00727968db7083d5f6))
* fixing type mismatches in fft impl ([88d39e5](http://github.com/benthevining/Limes/commit/88d39e5d8fbf109d8a5906721092ccdf7d4644c6))
* more SSE includes ([68f37dc](http://github.com/benthevining/Limes/commit/68f37dcdf2bd90eae27a943589b4c5a06a309676))
* not using Pommier SSE if compiler is MSVC ([af98b91](http://github.com/benthevining/Limes/commit/af98b91c6e3b2220830c72ce089e8e136b7210a4))
* type traits header ([2ab029a](http://github.com/benthevining/Limes/commit/2ab029a7f01b11779ae346a5cb674965d714a95c))


* reorganizing libs dependencies ([c631c2c](http://github.com/benthevining/Limes/commit/c631c2cea0a8413909d98f493cef8917bf40df9d))

## [2.7.0](http://github.com/benthevining/Limes/compare/v2.6.0...v2.7.0) (2022-04-07)


### Features

* added more vecops funcs ([ee4416b](http://github.com/benthevining/Limes/commit/ee4416b54fc3f482453dffe16fe549cd25c2d94c))

## [2.6.0](http://github.com/benthevining/Limes/compare/v2.5.0...v2.6.0) (2022-04-07)


### Features

* added FFTW wisdom integration ([ebabaa4](http://github.com/benthevining/Limes/commit/ebabaa4f809dd5f24c0a52672265d8925e19980e))

## [2.5.0](http://github.com/benthevining/Limes/compare/v2.4.0...v2.5.0) (2022-04-06)


### Features

* added more vecops funcs ([bdd7b79](http://github.com/benthevining/Limes/commit/bdd7b79a8efd86ef90e2d39d70e5cb48f4d10727))

## [2.4.0](http://github.com/benthevining/Limes/compare/v2.3.0...v2.4.0) (2022-04-05)


### Features

* added install types ([ad23b8d](http://github.com/benthevining/Limes/commit/ad23b8d653573082857793bff3f7915a78d324a5))

## [2.3.0](http://github.com/benthevining/Limes/compare/v2.2.0...v2.3.0) (2022-04-02)


### Features

* added an FFTW implementation to FFT wrapper ([9797618](http://github.com/benthevining/Limes/commit/979761812241c38e9d0bfe7e27a6086b15269cf8))

## [2.2.0](http://github.com/benthevining/Limes/compare/v2.1.0...v2.2.0) (2022-04-02)


### Features

* added a fallback FFT implementation ([61f8a36](http://github.com/benthevining/Limes/commit/61f8a366693f44211bedc2db88930410434e4724))


### Bug Fixes

* bug fixes in FFT impl ([9d3e66d](http://github.com/benthevining/Limes/commit/9d3e66d075902373c434163c377cdcbcd1bae558))
* FFT fixes ([ca71dfd](http://github.com/benthevining/Limes/commit/ca71dfdcb6a485af5e4744f0d8d34ace5b1334e3))
* FFT fixes ([0b680d6](http://github.com/benthevining/Limes/commit/0b680d65e5686fcc261cd039b83db609f3b07d27))

## [2.1.0](http://github.com/benthevining/Limes/compare/v2.0.4...v2.1.0) (2022-04-01)


### Features

* added an FFT wrapper to the vecops lib ([10ea3ab](http://github.com/benthevining/Limes/commit/10ea3ab4d6829df618868974223a2e59855d3263))

### [2.0.4](http://github.com/benthevining/Limes/compare/v2.0.3...v2.0.4) (2022-03-30)


### Bug Fixes

* added changelog to excluded files for typos hook ([fe1392b](http://github.com/benthevining/Limes/commit/fe1392be0b4d0349adabfb75afcd4469d063fddb))

### [2.0.3](http://github.com/benthevining/Limes/compare/v2.0.2...v2.0.3) (2022-03-30)


### Bug Fixes

* **deps:** pin dependencies ([1b261ba](http://github.com/benthevining/Limes/commit/1b261ba68a418aee32165e6e59eb97d793badfd1))

### [2.0.2](http://github.com/benthevining/Limes/compare/v2.0.1...v2.0.2) (2022-03-27)


### Bug Fixes

* fixed cppcheck precommit hook ([4680a0e](http://github.com/benthevining/Limes/commit/4680a0eb740106ffad384b74e424f3cef0caee1c))

### [2.0.1](http://github.com/benthevining/Limes/compare/v2.0.0...v2.0.1) (2022-03-26)


### Bug Fixes

* fixing makefile script ([2325ee7](http://github.com/benthevining/Limes/commit/2325ee713c965b054b90646272a2428d804fdd23))

## [2.0.0](http://github.com/benthevining/Limes/compare/v1.10.0...v2.0.0) (2022-03-21)


### ⚠ BREAKING CHANGES

* changed all code to be within limes:: namespace

Precommit-Verified: 9d371ba72e720a572e6d1d8144e71014265b56b7693adb8d9fdb6cb796074b28

* changed to limes namespace ([05d9481](http://github.com/benthevining/Limes/commit/05d9481ea3eb2055253039d4bec28c2949370449))

## [1.10.0](http://github.com/benthevining/Limes/compare/v1.9.0...v1.10.0) (2022-03-21)


### Features

* added export macro to all headers ([4e59dea](http://github.com/benthevining/Limes/commit/4e59deaba07d579f7ae664114822dfde6319d84b))

## [1.9.0](http://github.com/benthevining/Limes/compare/v1.8.0...v1.9.0) (2022-03-21)


### Features

* added a version header test ([3aac255](http://github.com/benthevining/Limes/commit/3aac25522d25014e4b27ed0df30f504ed8bcd3bf))


### Bug Fixes

* finding vecops dependencies in LimesConfig.cmake ([003465c](http://github.com/benthevining/Limes/commit/003465c45ff7324a8fd745db29e19bf65fd0cc2b))
* fixed MIPP integration ([4938277](http://github.com/benthevining/Limes/commit/4938277fea97d1a5188d57bc56842281b81bcac7))
* fixing includes in libs' CMakeLists ([e50bae1](http://github.com/benthevining/Limes/commit/e50bae18143fd2346bd3a3dc63e7bc81d75a57b4))
* fixing Oranges find module ([a2aae11](http://github.com/benthevining/Limes/commit/a2aae1199b205d74fd120006486c8a403456f3a9))

## [1.8.0](http://github.com/benthevining/Limes/compare/v1.7.1...v1.8.0) (2022-03-15)


### Features

* updating version in CMakeLists.txt with semantic release ([4f7c2af](http://github.com/benthevining/Limes/commit/4f7c2af633b460d519ac9da495fe2f8185834f32))

### [1.7.1](http://github.com/benthevining/Limes/compare/v1.7.0...v1.7.1) (2022-03-12)


### Bug Fixes

* added a LimesMacros.cmake file for installing the CMake modules ([055d2e9](http://github.com/benthevining/Limes/commit/055d2e929a112cd23988d3b96b46a2ecc968f9cb))

## [1.7.0](http://github.com/benthevining/Limes/compare/v1.6.0...v1.7.0) (2022-03-12)


### Features

* added CMake function to generate Version header ([d8f7d47](http://github.com/benthevining/Limes/commit/d8f7d47cad5a5ba28629de924bb872141197a192))

## [1.6.0](http://github.com/benthevining/Limes/compare/v1.5.0...v1.6.0) (2022-03-06)


### Features

* added Doxygen documentation build ([69bd5ce](http://github.com/benthevining/Limes/commit/69bd5ceaa387153967d9b524a327558d36d4d435))
* implemented cmake uninstall target ([d8a4d9e](http://github.com/benthevining/Limes/commit/d8a4d9e7b11987bcec6754d3ad003243a0afe541))


### Bug Fixes

* fixed another installation bug ([549a33b](http://github.com/benthevining/Limes/commit/549a33b90d2f863bd13ec07f3b1c54849652f79b))
* fixed bug with cmake install configuration ([dcf237e](http://github.com/benthevining/Limes/commit/dcf237e1ca559c19e3b67b5df709956e9df882b1))
* fixed cmake install ([27f05f0](http://github.com/benthevining/Limes/commit/27f05f02e87840deed6c5567cd05b0aebd4f666e))
* using better relative paths as include destinations in install ([141b770](http://github.com/benthevining/Limes/commit/141b77023f51f70b135c62f2eb39cf741d6e42c4))

## [1.5.0](http://github.com/benthevining/Limes/compare/v1.4.0...v1.5.0) (2022-03-04)


### Features

* added a MIPP implementation of all vecops functions ([743064b](http://github.com/benthevining/Limes/commit/743064b1b2dddcf2d11506e106690256593d1ef0))

## [1.4.0](http://github.com/benthevining/Limes/compare/v1.3.0...v1.4.0) (2022-03-04)


### Features

* added more math helper functions ([a92b0fd](http://github.com/benthevining/Limes/commit/a92b0fd694c502c7da30fd4edf48344112926c2b))

## [1.3.0](http://github.com/benthevining/Limes/compare/v1.2.0...v1.3.0) (2022-03-01)


### Features

* added more vecops functions ([bd2d3a7](http://github.com/benthevining/Limes/commit/bd2d3a74823ac853ac5e8d99833ce665a40ef8fd))

## [1.2.0](http://github.com/benthevining/Limes/compare/v1.1.0...v1.2.0) (2022-02-28)


### Features

* added more vecops functions ([fc2e647](http://github.com/benthevining/Limes/commit/fc2e647d30cafb38dce278aa61f934c82aeedf39))

## [1.1.0](http://github.com/benthevining/Limes/compare/v1.0.0...v1.1.0) (2022-02-28)


### Features

* implemented vecops functions for IPP ([25dc992](http://github.com/benthevining/Limes/commit/25dc9928a67b729f306322f03fcd766d65246714))

## 1.0.0 (2022-02-28)


### Features

* added a build step to the semantic release action ([906e1b7](http://github.com/benthevining/Limes/commit/906e1b7d5e9ea09c7becdf82d289a4a9f1c3f4c2))
* added a CodeQL scanning action ([48e301f](http://github.com/benthevining/Limes/commit/48e301f127803d0cfd8fd6864851103f7889b613))
* created readme ([500e96a](http://github.com/benthevining/Limes/commit/500e96a6d27ee97dae520ff7c6e5e9e4cbd9d55c))
* initial commit ([7e31891](http://github.com/benthevining/Limes/commit/7e318913119faa73ac67028dbb1f66fb9820ea17))


### Bug Fixes

* checking for GCC 10 in toplevel CMake ([da1077a](http://github.com/benthevining/Limes/commit/da1077a8f2f7215cbffebd621143de7a5d1505c0))
* fixing semantic release config ([1863111](http://github.com/benthevining/Limes/commit/1863111bf3de164d4d7e5596ae9cfc7a92b0d2ea))
* installing Ninja on Linux runners ([18a5aff](http://github.com/benthevining/Limes/commit/18a5aff8a2ad73d9eda1123e6e3df45541d0a114))
* sudo needed on Linux installation step ([1fdfbac](http://github.com/benthevining/Limes/commit/1fdfbac76f440d95743d097d5361e81b3e0d956d))
* using VS 2022 ([32edf2f](http://github.com/benthevining/Limes/commit/32edf2fa16f81b03e3f35511d9a497a9fc5a0064))
