/*
  ==============================================================================

    MidiChannelProcessor.cpp
    Created: 7 Mar 2021 2:34:46pm
    Author:  Trent

  ==============================================================================
*/

#include "MidiChannelProcessor.h"

MidiChannelProcessor::MidiChannelProcessor(std::string name, int channel) : channel_(channel), name_(name)
{
}

void MidiChannelProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void MidiChannelProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiBuffer)
{
    juce::MidiBuffer outBuffer;
    for (auto& i = midiBuffer.begin(); i != midiBuffer.end(); i++)
        if ((*i).getMessage().getChannel() == channel_)
        {
            outBuffer.addEvent((*i).getMessage(), 1);
        }

    midiBuffer.swapWith(outBuffer);
}

void MidiChannelProcessor::reset()
{
}

