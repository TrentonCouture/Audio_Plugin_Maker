/*
  ==============================================================================

    AudioProcessorFactory.h
    Created: 10 Feb 2021 7:44:14pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "ProcessorBase.h"
#include "OscillatorProcessor.h"
#include "FilterProcessor.h"
#include "ConvolutionProcessor.h"
#include "GainProcessor.h"
#include "MidiSineProcessor.h"
#include "MidiSawProcessor.h"
#include "MidiChannelProcessor.h"
#include "RmsProcessor.h"

class AudioProcessorFactory
{
public:
    std::unique_ptr<juce::AudioProcessor> getProcessor(std::string processorType, juce::var data);
};
