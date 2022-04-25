/*
  ==============================================================================

    PreviewSection.h
    Created: 20 Feb 2021 1:48:41pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "DspEditor.h"
#include "GuiEditor.h"
#include "Processor.h"
#include "Connector.h"

class PreviewSection : public juce::Component,
                       public juce::Button::Listener
{
public:
    PreviewSection(DspEditor& editor, GuiEditor& guiEditor);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;

private:
    juce::var setState(juce::OwnedArray<Processor>& processors, juce::OwnedArray<Connector>& connectors);
    void exportJSON(juce::var state, std::string filepath);

    DspEditor& editor_;
    GuiEditor& guiEditor_;

    juce::TextButton previewButton_;

};
