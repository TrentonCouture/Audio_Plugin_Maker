/*
  ==============================================================================

    MidiChannelProcessor.h
    Created: 7 Mar 2021 2:34:46pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "ProcessorBase.h"

class MidiChannelProcessor  : public ProcessorBase
{
public:
    MidiChannelProcessor(std::string name, int channel);
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override;
    void reset() override;

    const juce::String getName() const override { return name_; }

private:
    int channel_;
    std::string name_;
};
