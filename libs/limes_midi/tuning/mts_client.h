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

#include <libMTSClient.h>
#include <memory>
#include <string>
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace midi
{

class LIMES_EXPORT MtsClient final
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

}  // namespace midi

LIMES_END_NAMESPACE
