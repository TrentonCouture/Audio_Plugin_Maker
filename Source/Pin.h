/*
  ==============================================================================

    Pin.h
    Created: 18 Feb 2021 7:48:26pm
    Author:  Trent

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DspEditor;
class Processor;

class Pin : public juce::Component
{
public:
    Pin(DspEditor& editor, Processor& processor, bool isInput, bool isMidi, int pinNum);
    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

    Processor* getProcessor();

    bool isInput();

    int getPinNum();
    bool isMidi();

private:
    int pinNum_;
    bool isInput_;
    bool isMidi_;
    DspEditor& editor_;
    Processor& processor_;

};
