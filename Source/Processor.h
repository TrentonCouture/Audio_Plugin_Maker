/*
  ==============================================================================

    ProcessorComponent.h
    Created: 15 Feb 2021 3:54:43pm
    Author:  Trent

  ==============================================================================
*/ 

#pragma once
#include <JuceHeader.h>

#include "Pin.h"
#include "ProcessorConfig.h"

class DspEditor;

class Processor: public juce::Component
{
public:
    Processor(DspEditor& editor, juce::String type, juce::String id, ProcessorConfig config);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void makePopupMenu(juce::Point<int> position);
    juce::Point<float> getPinPoint(int index, bool isInput);
    juce::String getType();
    juce::String getID();
    bool shouldBeDeleted();
    bool hitTest(int x, int y) override;
    void setPosition(float x, float y);
    juce::NamedValueSet getParameters();
    void setParameter(juce::String name, float value);
    int getNumMidiInputPins();
    int getNumMidiOutputPins();
    int getNumAudioIn();
    int getNumAudioOut();

private:
    DspEditor& editor_;
    juce::Point<int> originalPos_;
    juce::Point<float> position_;
    juce::String type_;
    juce::String id_;
    std::unique_ptr<juce::PopupMenu> popupMenu_;
    bool deleteMe_;
    juce::OwnedArray<Pin> pins;
    int pinWidth_ = 16;
    int pinHeight_ = 4;
    juce::DynamicObject parameters_;
    int numMidiIn_;
    int numMidiOut_;
    int numAudioIn_;
    int numAudioOut_;

};
