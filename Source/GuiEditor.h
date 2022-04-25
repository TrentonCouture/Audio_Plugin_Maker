/*
  ==============================================================================

    GuiEditor.h
    Created: 24 Mar 2021 6:29:57pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "MovableSlider.h"
#include "MovableButton.h"

class GuiEditor : public juce::Component
{
public:
    GuiEditor();
    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;
    void makePopupMenu(juce::Point<int> position);
    juce::var setState();
    void exportJSON(juce::String filepath);
private:
    std::unique_ptr<juce::PopupMenu> popupMenu_;
    juce::OwnedArray<MovableSlider> sliders_;
    juce::OwnedArray<MovableButton> buttons_;
    int newId_;
};
