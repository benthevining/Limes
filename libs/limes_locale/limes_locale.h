/*
 * ======================================================================================
 *  __    ____  __  __  ____  ___
 * (  )  (_  _)(  \/  )( ___)/ __)
 *  )(__  _)(_  )    (  )__) \__ \
 * (____)(____)(_/\/\_)(____)(___/
 *
 *  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

/** @defgroup limes_locale limes_locale
	The Limes library of localization utilities.

	All classes and functions in this module are accessible after linking to the \c limes_locale library and including limes_locale.h.

	This library provides a built-in database of metadata about various languages and countries.
	ISO specifications are used where applicable, though every effort has been made to include information on Indigenous languages and contested states.

	Dependencies: limes_data_structures

	@ingroup limes
 */

/** @dir libs/limes_locale
	This directory contains the limes_locale library.
	@ingroup limes_locale
 */

/** @file
	The main header for the limes_locale library.
	@ingroup limes_locale
 */

#pragma once

// IWYU pragma: begin_exports

#include "languages/languages.h"
#include "countries/countries.h"

// IWYU pragma: end_exports
