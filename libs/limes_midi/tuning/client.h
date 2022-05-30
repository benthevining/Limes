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

#include <limes_core.h>

LIMES_DISABLE_ALL_COMPILER_WARNINGS

#include <libMTSClient.h>

LIMES_REENABLE_ALL_COMPILER_WARNINGS

#include <memory>
#include <string>
#include <limes_namespace.h>

/** @defgroup midi_tuning Tuning
	Utilities for working with MIDI tuning systems.
	Limes provides C++ wrappers for the MTS-ESP client and master libraries.
	@ingroup limes_midi
 */

/** @dir libs/limes_midi/tuning
	This directory contains utilities for working with MIDI tuning systems.
	@ingroup midi_tuning
 */

/** @file
	This file defines the MIDI tuning Client class.
	@ingroup midi_tuning
 */

LIMES_BEGIN_NAMESPACE

/** This namespace contains utilities for working with MIDI tuning systems.
	@ingroup midi_tuning
 */
namespace midi::tuning
{

/** This class is a simple wrapper around the MTS-ESP MIDI tuning library.

	This class represents a %client in an MTS session, and can receive tuning data from the session master.

	@ingroup midi_tuning
 */
class LIMES_EXPORT Client final
{
public:

	/** Converts a given MIDI note to a frequency in Hz, using the master tuning table.
		@param midiNote The MIDI pitch to convert.
		@param midiChannel The MIDI channel of the tuning table to use. You can specify -1 for "all" or "none".
	*/
	[[nodiscard]] float midiToFrequency (int midiNote, int midiChannel = -1) const;

	/** Converts a given frequency in Hz to a MIDI note, using the master tuning table.
		@param frequency The frequency in Hz to convert.
		@param midiChannel The MIDI channel of the tuning table to use. You can specify -1 for "all" or "none".
	*/
	[[nodiscard]] float frequencyToMidi (double frequency, int midiChannel = -1) const;

	/** Returns the retune amount in semitones for a given MIDI pitch.
		@param midiNote The MIDI pitch to get the retuning amount for.
		@param midiChannel The MIDI channel of the tuning table to use. You can specify -1 for "all" or "none".
	*/
	[[nodiscard]] float retuneAmountInSemitones (int midiNote, int midiChannel = -1) const;

	/** MTS-ESP supports "filtering" some notes out of tuning tables.
		Call this function to detect if a certain pitch should be filtered out or ignored.
		@param midiPitch The MIDI note to check.
		@param midiChannel The MIDI channel of the tuning table to use. You can specify -1 for "all" or "none".
		@return True if the passed pitch should be ignored/filtered; false otherwise.
	*/
	[[nodiscard]] bool shouldFilterNote (int midiPitch, int midiChannel = -1) const;

	/** Returns true if this client is currently connected to an MTS-ESP session master. */
	[[nodiscard]] bool isConnected() const;

	/** Returns the scale name specified by the MTS-ESP session master. This may return an empty string. */
	[[nodiscard]] std::string getScaleName() const;

private:

	std::unique_ptr<MTSClient, decltype ([] (MTSClient* c)
										 { MTS_DeregisterClient (c); })>
		client { MTS_RegisterClient() };
};

}  // namespace midi::tuning

LIMES_END_NAMESPACE
