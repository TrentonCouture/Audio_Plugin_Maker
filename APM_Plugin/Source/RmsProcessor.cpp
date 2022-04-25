/*
  ==============================================================================

    RmsProcessor.cpp
    Created: 5 Apr 2021 6:23:51pm
    Author:  Trent

  ==============================================================================
*/

#include "RmsProcessor.h"

RmsProcessor::RmsProcessor(std::string name, juce::var config) : name_(name), config_(config)
{
}

void RmsProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void RmsProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiBuffer)
{
    // shouldn't be dependent on the buffer size...

    float sum = 0;
    for (int channel = 0; channel < getTotalNumInputChannels(); channel++)
    {
        auto readPtr = buffer.getReadPointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); i++)
            sum += pow(readPtr[i], 2);
    }

    auto power = sum / (2*buffer.getNumSamples());
    auto rms = sqrt(power);

    auto midiVal = rms * 2 * 127 - 127;

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
        juce::MidiMessage midiMessage = juce::MidiMessage::controllerEvent(channel, 0, midiVal);
        midiBuffer.addEvent(midiMessage, 1);
    }

}

void RmsProcessor::reset()
{
}

