/*
  ==============================================================================

    FilterProcessor.cpp
    Created: 9 Feb 2021 8:32:42pm
    Author:  Trent

  ==============================================================================
*/

#include "FilterProcessor.h"

FilterProcessor::FilterProcessor(std::string name, juce::var config) 
    : name_(name), cutoff_(1000), resonance_(0.5), sampleRate_(0), config_(config) {}


void FilterProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sampleRate_ = sampleRate;
    *filter_.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, cutoff_, resonance_);

    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), getNumInputChannels() };
    filter_.prepare (spec);
}

void FilterProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiBuffer)
{
    int numInputs = 0;
    int midiIn = 0;
    for (auto& i = midiBuffer.begin(); i != midiBuffer.end(); i++)
        if ((*i).getMessage().isController() && (*i).getMessage().getChannel() == 1)
        {
            midiIn += (*i).getMessage().getControllerValue();
            numInputs++;
        }

    if (numInputs > 0)
        cutoff_ = 440.0 * pow(2, (((float)midiIn / (float)numInputs) - 69.0) / 12.0);

    numInputs = 0;
    midiIn = 0;
    for (auto& i = midiBuffer.begin(); i != midiBuffer.end(); i++)
        if ((*i).getMessage().isController() && (*i).getMessage().getChannel() == 2)
        {
            midiIn += (*i).getMessage().getControllerValue();
            numInputs++;
        }

    if (numInputs > 0)
        resonance_ = (float)midiIn / (float)numInputs + .001;

    *filter_.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate_, cutoff_, resonance_);
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    filter_.process (context);
}

void FilterProcessor::reset()
{
    filter_.reset();
}

void FilterProcessor::setCutoff(float cutoff)
{
    cutoff_ = cutoff;
}

const juce::String FilterProcessor::getName() const
{
    return name_;
}
