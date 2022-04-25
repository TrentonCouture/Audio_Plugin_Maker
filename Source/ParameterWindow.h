/*
  ==============================================================================

    ParameterWindow.h
    Created: 23 Feb 2021 8:56:31pm
    Author:  Trent

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ParameterDisplay.h"

class DspEditor;
class Processor;

class ParameterWindow : public juce::DocumentWindow
{
public:
    ParameterWindow(DspEditor& editor, Processor& processor, const juce::String& name, juce::Colour backgroundColour, int requiredButtons, bool addToDesktop = true);
    void closeButtonPressed() override;
private:
    DspEditor& editor_;
    juce::OwnedArray<juce::Label> labels_;
};
