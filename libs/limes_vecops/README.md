<!-- markdownlint-disable -->
```
 __    ____  __  __  ____  ___
(  )  (_  _)(  \/  )( ___)/ __)
 )(__  _)(_  )    (  )__) \__ \
(____)(____)(_/\/\_)(____)(___/
```

# limes_vecops

A library for SIMD-accelerated vector operations

## Dependencies

* [limes_core](../limes_core/README.md)

## Targets

* Limes::limes_vecops

## Install components

* limes_vecops_dev
* limes_vecops_runtime
* limes_vecops (group)

## CMake options

This library is designed to be a wrapper around various platform- and vendor-specific hardware accelerated libraries.

The following backends are currently available:

* [Apple Accelerate (vDSP)](https://developer.apple.com/documentation/accelerate/vdsp)
* [Intel IPP](https://www.intel.com/content/www/us/en/developer/tools/oneapi/ipp.html)
* [MIPP](https://github.com/aff3ct/MIPP)
* Fallback (scalar implementations of all functions)

Additionally, an [FFTW](https://www.fftw.org/) backend for this library's FFT class is available.

If you specify no options, limes_vecops will search for an available backend library in this order, selecting the first one it finds:

* Apple Accelerate, if on an Apple platform
* Intel IPP, if on an Intel platform
* MIPP
* Fallback

The default behavior is to use FFTW for FFT's, if it can be found.

The following CMake options are available to give you more control over which backend is selected:

* LIMES_IGNORE_VDSP
* LIMES_IGNORE_IPP
* LIMES_IGNORE_MIPP
* LIMES_USE_IPP
* LIMES_USE_MIPP
* LIMES_USE_VECOPS_FALLBACK
* LIMES_IGNORE_POMMIER
* LIMES_IGNORE_FFTW
