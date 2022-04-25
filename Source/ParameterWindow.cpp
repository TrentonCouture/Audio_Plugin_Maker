/*
  ==============================================================================

    ParameterWindow.cpp
    Created: 23 Feb 2021 8:56:31pm
    Author:  Trent

  ==============================================================================
*/

#include "ParameterWindow.h"
#include "DspEditor.h"
#include "Processor.h"

ParameterWindow::ParameterWindow(DspEditor& editor, Processor& processor, const juce::String& name, juce::Colour backgroundColour, int requiredButtons, bool addToDesktop)
    : DocumentWindow(name, backgroundColour, requiredButtons, addToDesktop), editor_(editor)
{
    setContentOwned(new ParameterDisplay(processor), true);
}

void ParameterWindow::closeButtonPressed()
{
    editor_.deleteParamWindow();
}
