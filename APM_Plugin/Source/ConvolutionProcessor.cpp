/*
  ==============================================================================

    ConvolutionProcessor.cpp
    Created: 22 Mar 2021 8:06:38pm
    Author:  Trent

  ==============================================================================
*/

#include "ConvolutionProcessor.h"

ConvolutionProcessor::ConvolutionProcessor(std::string name, juce::var config) : name_(name)
{
}

void ConvolutionProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void ConvolutionProcessor::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiBuffer)
{
    //buffer.copyFrom(0, 0, buffer.getReadPointer(2), buffer.getNumSamples());
    //buffer.copyFrom(1, 0, buffer.getReadPointer(3), buffer.getNumSamples());
}

void ConvolutionProcessor::reset()
{
}

