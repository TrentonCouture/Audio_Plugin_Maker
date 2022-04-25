/*
  ==============================================================================

    FilterProcessor.h
    Created: 9 Feb 2021 8:32:42pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "ProcessorBase.h"

class FilterProcessor  : public ProcessorBase
{
public:
    FilterProcessor(std::string name, juce::var config);
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override;
    void reset() override;

    void setCutoff(float cutoff);

    const juce::String getName() const override;

private:
    std::string name_;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter_;

    float cutoff_;
    float resonance_;
    float sampleRate_;

    juce::var config_;
};
