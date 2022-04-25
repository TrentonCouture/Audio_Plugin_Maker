/*
  ==============================================================================

    MidiSineProcessor.h
    Created: 28 Feb 2021 7:49:09pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "ProcessorBase.h"

class MidiSineProcessor  : public ProcessorBase
{
public:
    MidiSineProcessor(std::string name, juce::var config);
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override;
    void reset() override;

    void setFreq(float freq);

    const juce::String getName() const override { return name_; }

private:
    std::string name_;
    juce::var config_;
    juce::dsp::Oscillator<float> oscillator;
};
