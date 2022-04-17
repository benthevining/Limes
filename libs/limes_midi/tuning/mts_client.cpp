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

#include "mts_client.h"

namespace limes::midi
{

float midiToFrequency (int midiNote, int midiChannel) const
{
	return static_cast<float> (MTS_NoteToFrequency (client.get(),
													char (midiNote),
													char (midiChannel)));
}

float frequencyToMidi (double frequency, int midiChannel) const
{
	return static_cast<float> (MTS_FrequencyToNote (client.get(),
													frequency,
													char (midiChannel)));
}

float retuneAmountInSemitones (int midiNote, int midiChannel) const
{
	return static_cast<float> (MTS_RetuningInSemitones (client.get(),
														char (midiNote),
														char (midiChannel)));
}

bool MtsClient::shouldFilterNote (int midiPitch, int midiChannel) const
{
	return MTS_ShouldFilterNote (client.get(), char (midiPitch), char (midiChannel));
}

bool MtsClient::isConnected() const
{
	return MTS_HasMaster (client.get());
}

std::string MtsClient::getScaleName() const
{
	if (! isConnected())
		return {};

	return { MTS_GetScaleName (client.get()) };
}

// void MtsClient::parseMidiMessageForSysEx (const juce::MidiMessage& m) const
// {
// 	if (! m.isSysEx())
// 		return;

// 	MTS_ParseMIDIDataU (client.get(),
// 						m.getSysExData(),
// 						m.getSysExDataSize());
// }


}  // namespace limes::midi
