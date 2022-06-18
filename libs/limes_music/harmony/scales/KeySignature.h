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

#include <limes_export.h>	// for LIMES_EXPORT
#include "Scale.h"			// for Scale
#include "../PitchClass.h"	// for PitchClass
#include <limes_namespace.h>
#include <limes_core.h>

/** @file
	This file defines the KeySignature %scale class.
	@ingroup music_scales
 */

LIMES_BEGIN_NAMESPACE

namespace music::scales
{

class Mode;

/** This class represents any traditional Western key signature (eg, major, natural minor, harmonic minor).
	Theoretical keys (those containing more than 7 sharps or flats) are currently not supported.

	For easy reference, here is a table of all possible key signatures:

	Sharp keys:

	Number of sharps | Major key     | Minor key
	---------------- | ------------- | ---------------
	0                | C major       | A minor
	1                | G major       | E minor
	2                | D major       | B minor
	3                | A major       | F-sharp minor
	4                | E major       | C-sharp minor
	5                | B major       | G-sharp minor
	6                | F-sharp major | D-sharp minor
	7                | C-sharp major | A-sharp minor

	Flat keys:

	Number of sharps | Major key     | Minor key
	---------------- | ------------- | ---------------
	0                | C major       | A minor
	1                | F major       | D minor
	2                | B-flat major  | G minor
	3                | E-flat major  | C minor
	4                | A-flat major  | F minor
	5                | D-flat major  | B-flat minor
	6                | G-flat major  | E-flat minor
	7                | C-flat major  | A-flat minor

	@ingroup music_scales
 */
class LIMES_EXPORT KeySignature final : public Scale
{
public:

	/** Represents the type of this key signature. */
	enum class Type
	{
		Major,
		NaturalMinor,
		HarmonicMinor
		// MelodicMinor - not yet supported
	};

	/** @name Constructors */
	///@{

	/** Creates a key signature object representing C major. */
	constexpr KeySignature() noexcept;

	/** Creates a key signature with a given root.
		@param typeToUse The type of this key signature.
		@param isSharps If true, sharps will be used; otherwise, flats will be used.
		@param rootNote The MIDI %pitch of the scale's root note.
	 */
	constexpr explicit KeySignature (Type typeToUse, bool isSharps, int rootNote) noexcept;

	/** Creates a key signature with a given root.
		@param typeToUse The type of this key signature.
		@param isSharps If true, sharps will be used; otherwise, flats will be used.
		@param pitchClassOfRoot The %pitch class of the scale's root.
	 */
	constexpr explicit KeySignature (Type typeToUse, bool isSharps, const PitchClass& pitchClassOfRoot) noexcept;

	/** Creates a key signature with a given root.
		This constructor infers whether to use sharps or flats based on the root and type.
		@param typeToUse The type of this key signature.
		@param rootPitch The MIDI %pitch of the scale's root.
	 */
	constexpr explicit KeySignature (Type typeToUse, int rootPitch) noexcept;

	/** Creates a key signature with a given root.
		This constructor infers whether to use sharps or flats based on the root and type.
		@param typeToUse The type of this key signature.
		@param pitchClassOfRoot The %pitch class of the scale's root, as an integer between 0 and 11.
	 */
	constexpr explicit KeySignature (Type typeToUse, const PitchClass& pitchClassOfRoot) noexcept;

	///@}

	LIMES_DEFAULT_COPYABLE (KeySignature)
	LIMES_DEFAULT_MOVABLE (KeySignature)

	/** Returns true if the other key signature is semantically equivalent to this one. */
	[[nodiscard]] constexpr bool operator== (const KeySignature& other) const noexcept;

	/** Returns true if the other key signature is not semantically equivalent to this one. */
	[[nodiscard]] constexpr bool operator!= (const KeySignature& other) const noexcept;

	/** Returns a key signature object representing the relative key of this key.
		For example, if your key signature is D minor, this function will return F major.
		@see isRelativeKeyOf()
	 */
	[[nodiscard]] KeySignature getRelativeKey() const noexcept;

	/** Returns true if this key signature object represents the relative key of the other one.
		@see getRelativeKey()
	 */
	[[nodiscard]] constexpr bool isRelativeKeyOf (const KeySignature& other) const noexcept;

	/** Returns a key signature object representing the parallel key of this key.
		For example, if your key signature is C major, this function will return C minor.
		@see isParallelKeyOf()
	 */
	[[nodiscard]] KeySignature getParallelKey() const noexcept;

	/** Returns true if this key signature object represents the parallel key of the other one.
		@see getParallelKey()
	 */
	[[nodiscard]] bool isParallelKeyOf (const KeySignature& other) const noexcept;

	/** Returns true if this key has an enharmonic equivalent that can be spelled using 7 or less flats or sharps.
		@see getEnharmonicKey(), isEnharmonicKeyOf()
	 */
	[[nodiscard]] bool hasEnharmonicKey() const noexcept;

	/** If this key has an enharmonic, this function returns it. If not, this function returns a copy of the current key.
		@see hasEnharmonicKey(), isEnharmonicKeyOf()
	 */
	[[nodiscard]] KeySignature getEnharmonicKey() const noexcept;

	/** Returns true if this key signature object represents the enharmonic key of the other one.
		@see getEnharmonicKey(), hasEnharmonicKey()
	 */
	[[nodiscard]] bool isEnharmonicKeyOf (const KeySignature& other) const noexcept;

	/** Returns the major key whose root is a perfect fifth above this key's root.
		@see isDominantKeyOf()
	 */
	[[nodiscard]] KeySignature getDominantKey() const noexcept;

	/** Returns true if this key signature object represents the dominant key of the other one.
		@see getDominantKey()
	 */
	[[nodiscard]] bool isDominantKeyOf (const KeySignature& other) const noexcept;

	/** Returns true if this key is spelled using flats. */
	[[nodiscard]] constexpr bool isFlatKey() const noexcept;

	/** Returns true if this key is spelled using sharps. */
	[[nodiscard]] constexpr bool isSharpKey() const noexcept;

	/** Returns true if this key is major. */
	[[nodiscard]] constexpr bool isMajorKey() const noexcept;

	/** Returns true if this key is minor. */
	[[nodiscard]] constexpr bool isMinorKey() const noexcept;

	/** Returns the type of this key signature. */
	[[nodiscard]] constexpr Type getKeyType() const noexcept;

	/** Returns the %pitch class of this scale's root. */
	[[nodiscard]] PitchClass getPitchClassOfRoot() const noexcept final;

	/** If this key is spelled with sharps, this returns the number of sharps in the key. Otherwise this returns 0. */
	[[nodiscard]] constexpr int getNumSharps() const noexcept;

	/** If this key is spelled with flats, this returns the number of flats in the key. Otherwise this returns 0. */
	[[nodiscard]] constexpr int getNumFlats() const noexcept;

	/** Returns the set of intervals that make up this key signature. */
	[[nodiscard]] std::vector<int> getIntervalsAsSemitones() const final;

	/** Returns 8. */
	[[nodiscard]] int notesPerOctave() const noexcept final;

private:

	constexpr explicit KeySignature (int numSharpsOrFlats, bool isSharps, Type typeToUse) noexcept;

	[[nodiscard]] static constexpr bool useSharpsForRootByDefault (const PitchClass& root) noexcept;

	[[nodiscard]] static constexpr bool rootHasEnharmonicKey (int root) noexcept;

	[[nodiscard]] constexpr bool hasOppositeTonality (const KeySignature& other) const noexcept;

	int numAccidentals { 0 };

	bool isFlat { false };

	Type type { Type::Major };

	friend class Mode;
};

}  // namespace music::scales

LIMES_END_NAMESPACE

#include "KeySignature_impl.h"	// IWYU pragma: export
