/*
  ==============================================================================

    OscillatorProcessor.cpp
    Created: 9 Feb 2021 7:44:47pm
    Author:  Trent

  ==============================================================================
*/

#include "OscillatorProcessor.h"

OscillatorProcessor::OscillatorProcessor(std::string name) : name_(name)
{
    oscillator.setFrequency (440.0f);
    oscillator.initialise ([] (float x) { return 0.1 * x / juce::MathConstants<float>::pi; });
}

void OscillatorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock) };
    oscillator.prepare (spec);
}

void OscillatorProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiBuffer)
{
    int numInputs = 0;
    int midiIn = 0;
    for (auto& i = midiBuffer.begin(); i != midiBuffer.end(); i++)
        if ((*i).getMessage().isController())
        {
            midiIn += (*i).getMessage().getControllerValue();
            numInputs++;
        }

    if (numInputs > 0)
        oscillator.setFrequency(440.0 * pow(2, (((float)midiIn / (float)numInputs) - 69.0) / 12.0));

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    oscillator.process (context);
}

void OscillatorProcessor::reset()
{
    oscillator.reset();
}


void OscillatorProcessor::setFreq(float freq)
{
    oscillator.setFrequency(freq);
}
