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

#include "./mts_client.h"
#include <limes_namespace.h>

LIMES_BEGIN_NAMESPACE

namespace midi::tuning
{

float Client::midiToFrequency (int midiNote, int midiChannel) const
{
	return static_cast<float> (MTS_NoteToFrequency (client.get(),
													char (midiNote),
													char (midiChannel)));
}

float Client::frequencyToMidi (double frequency, int midiChannel) const
{
	return static_cast<float> (MTS_FrequencyToNote (client.get(),
													frequency,
													char (midiChannel)));
}

float Client::retuneAmountInSemitones (int midiNote, int midiChannel) const
{
	return static_cast<float> (MTS_RetuningInSemitones (client.get(),
														char (midiNote),
														char (midiChannel)));
}

bool Client::shouldFilterNote (int midiPitch, int midiChannel) const
{
	return MTS_ShouldFilterNote (client.get(), char (midiPitch), char (midiChannel));
}

bool Client::isConnected() const
{
	return MTS_HasMaster (client.get());
}

std::string Client::getScaleName() const
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


}  // namespace midi::tuning

LIMES_END_NAMESPACE
