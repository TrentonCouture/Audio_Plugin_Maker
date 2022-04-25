/*
  ==============================================================================

    GainProcessor.h
    Created: 9 Feb 2021 8:29:39pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "ProcessorBase.h"

class GainProcessor  : public ProcessorBase
{
public:
    GainProcessor(std::string name);
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override;
    void reset() override;
    const juce::String getName() const override;

private:
    std::string name_;
    juce::dsp::Gain<float> gain;
};
