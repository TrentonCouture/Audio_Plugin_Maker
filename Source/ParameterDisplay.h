/*
  ==============================================================================

    ParameterDisplay.h
    Created: 27 Feb 2021 8:16:07pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Processor;

class ParameterDisplay : public juce::Component
{
public:
    ParameterDisplay(Processor& processor);
    void resized() override;
private:
    Processor& processor_;
    juce::OwnedArray<juce::Label> labels_;
    juce::OwnedArray<juce::Label> inputLabels_;
};
