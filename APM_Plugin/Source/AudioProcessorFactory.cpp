/*
  ==============================================================================

    AudioProcessorFactory.cpp
    Created: 10 Feb 2021 7:44:14pm
    Author:  Trent

  ==============================================================================
*/

#include "AudioProcessorFactory.h"


std::unique_ptr<juce::AudioProcessor> AudioProcessorFactory::getProcessor(std::string processorId, juce::var data)
{
    if (data["type"].toString() == "Oscillator")
    {
        auto processorPtr = std::make_unique<OscillatorProcessor>(processorId);
        processorPtr->setFreq(float(data["freq"]));
        return std::move(processorPtr);
    }
    else if (data["type"].toString() == "Gain")
        return std::move(std::make_unique<GainProcessor>(processorId));
    else if (data["type"].toString() == "Filter")
    {
        auto processorPtr = std::make_unique<FilterProcessor>(processorId, data);
        processorPtr->setCutoff(float(data["cutoff"]));
        return std::move(processorPtr);
    }
    else if (data["type"].toString() == "RMS")
        return std::move(std::make_unique<RmsProcessor>(processorId, data));
    else if (data["type"].toString() == "Convolution")
        return std::move(std::make_unique<ConvolutionProcessor>(processorId, data));
    else if (data["type"].toString() == "MidiSine")
        return std::move(std::make_unique<MidiSineProcessor>(processorId, data));
    else if (data["type"].toString() == "MidiSaw")
        return std::move(std::make_unique<MidiSawProcessor>(processorId));
    else if (data["type"].toString() == "Output")
        return std::move(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
    else if (data["type"].toString() == "Input")
        return std::move(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
    else if (data["type"].toString() == "MidiOutput")
        return std::move(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::midiOutputNode));
    else if (data["type"].toString() == "MidiInput")
        return std::move(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(juce::AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode));
    else if (data["type"].toString() == "MidiChannelProcessor")
        return std::move(std::make_unique<MidiChannelProcessor>(processorId, data["channel"]));
}
