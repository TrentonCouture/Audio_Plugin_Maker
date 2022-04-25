/*
  ==============================================================================

    MovableButton.h
    Created: 14 Apr 2021 6:25:39pm
    Author:  Trent

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MovableButton : public juce::ShapeButton
{
public:
    MovableButton(juce::String id);
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
private:	
    juce::String ID_;
    //juce::ShapeButton button_;

    juce::Point<int> originalPos_;
    juce::Point<float> position_;
};
