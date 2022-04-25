/*
  ==============================================================================

    MidiSawProcessor.h
    Created: 1 Mar 2021 5:38:03pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "ProcessorBase.h"
#include <math.h>

class MidiSawProcessor  : public ProcessorBase
{
public:
    MidiSawProcessor(std::string name);
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override;
    void reset() override;

    void setFreq(float freq);

    const juce::String getName() const override { return name_; }

private:
    std::string name_;
    juce::dsp::Oscillator<float> oscillator;
};
