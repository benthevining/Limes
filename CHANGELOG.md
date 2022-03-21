# Limes changelog

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
