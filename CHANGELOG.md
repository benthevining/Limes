# Limes changelog

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
