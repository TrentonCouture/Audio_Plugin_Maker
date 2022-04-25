/*
  ==============================================================================

    RmsProcessor.h
    Created: 5 Apr 2021 6:23:51pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "ProcessorBase.h"

class RmsProcessor  : public ProcessorBase
{
public:
    RmsProcessor(std::string name, juce::var config);
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override;
    void reset() override;

    const juce::String getName() const override { return name_; }

private:
    std::string name_;
    juce::var config_;
};
