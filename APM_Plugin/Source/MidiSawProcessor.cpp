/*
  ==============================================================================

    MidiSawProcessor.cpp
    Created: 1 Mar 2021 5:38:03pm
    Author:  Trent

  ==============================================================================
*/

#include "MidiSawProcessor.h"

MidiSawProcessor::MidiSawProcessor(std::string name) : name_(name)
{
    oscillator.setFrequency (10.0f);
    oscillator.initialise ([] (float x) { return (127/2) * x / juce::MathConstants<float>::pi + (127/2); });
}

void MidiSawProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock) };
    oscillator.prepare (spec);
}

void MidiSawProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiBuffer)
{
    float sample = 0;
    sample = oscillator.processSample(sample);
    juce::MidiMessage midiMessage = juce::MidiMessage::controllerEvent(1, 0, sample);
    midiBuffer.addEvent(midiMessage, 1);
}

void MidiSawProcessor::reset()
{
    oscillator.reset();
}

void MidiSawProcessor::setFreq(float freq)
{
    oscillator.setFrequency(freq);
}
