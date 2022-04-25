/*
  ==============================================================================

    OscillatorProcessor.h
    Created: 9 Feb 2021 7:44:47pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "ProcessorBase.h"

class OscillatorProcessor  : public ProcessorBase
{
public:
    OscillatorProcessor(std::string name);
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override;
    void reset() override;

    void setFreq(float freq);

    const juce::String getName() const override { return name_; }

private:
    std::string name_;
    juce::dsp::Oscillator<float> oscillator;
};
