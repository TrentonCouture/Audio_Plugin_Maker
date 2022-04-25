/*
  ==============================================================================

    MovableSlider.h
    Created: 11 Apr 2021 3:08:25pm
    Author:  Trent

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MovableSlider : public juce::Slider
{
public:
    MovableSlider(juce::String id);
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void resized() override;

    juce::Point<float> getPosition();
    juce::String getID();
private:
    juce::Point<int> originalPos_;
    juce::Point<float> position_;

    juce::String ID_;
};
