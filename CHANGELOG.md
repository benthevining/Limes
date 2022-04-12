# Limes changelog

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
