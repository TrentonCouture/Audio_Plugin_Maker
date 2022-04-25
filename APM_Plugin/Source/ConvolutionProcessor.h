/*
  ==============================================================================

    ConvolutionProcessor.h
    Created: 22 Mar 2021 8:06:38pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "ProcessorBase.h"

class ConvolutionProcessor  : public ProcessorBase
{
public:
    ConvolutionProcessor(std::string name, juce::var config);
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override;
    void reset() override;

    const juce::String getName() const override { return name_; }

private:
    std::string name_;
};
