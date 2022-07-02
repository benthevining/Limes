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

#pragma once

#include <limes_export.h>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace music
{

struct LIMES_EXPORT TimeSignature final
{
public:
	struct Subdivision final
	{
		int numBeats;

		NoteDuration beatKind;
	};

	/*
	- get number of notes of a certain duration in a measure

	*/

private:
};

}  // namespace music

LIMES_END_NAMESPACE
