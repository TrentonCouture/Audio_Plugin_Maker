/*
  ==============================================================================

    MidiSineProcessor.cpp
    Created: 28 Feb 2021 7:49:10pm
    Author:  Trent

  ==============================================================================
*/

#include "MidiSineProcessor.h"

MidiSineProcessor::MidiSineProcessor(std::string name, juce::var config) : name_(name), config_(config)
{
    oscillator.setFrequency (100.0f);
    oscillator.initialise ([] (float x) { return (127/2) * std::sin(x) + (127/2); });
}

void MidiSineProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock) };
    oscillator.prepare (spec);
}

void MidiSineProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiBuffer)
{
    oscillator.setFrequency(config_["frequency"]);
    float sample = 0;
    sample = oscillator.processSample(sample);

    std::unordered_set<int> outChannels = {};

    for (auto& outArr : *config_["midiOut"].getArray())
    {
        for (auto& outVals : *outArr.getArray())
        {
            auto pos = outVals.toString().toStdString().find(":");
            outChannels.insert(std::stoi(outVals.toString().toStdString().substr(pos + 1)));
        }
    }
    for (auto& channel : outChannels)
    {
        juce::MidiMessage midiMessage = juce::MidiMessage::controllerEvent(channel, 0, sample);
        midiBuffer.addEvent(midiMessage, 1);
    }
}

void MidiSineProcessor::reset()
{
    oscillator.reset();
}


void MidiSineProcessor::setFreq(float freq)
{
    oscillator.setFrequency(freq);
}
