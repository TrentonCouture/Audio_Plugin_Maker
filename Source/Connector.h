/*
  ==============================================================================

    Connector.h
    Created: 19 Feb 2021 6:55:56pm
    Author:  Trent

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Processor.h"

class DspEditor;

class Connector : public juce::Component
{
public:
    Connector(DspEditor* editor);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void resizedToFit();

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

    void getPoints(juce::Point<float>& p1, juce::Point<float>& p2);

    void setSource(Pin* source);
    void setDestination(Pin* destination);
    Pin* getSource();
    Pin* getDestination();

    void dragStart(juce::Point<float> pos);
    void dragEnd(juce::Point<float> pos);

    void update();

    bool hitTest(int x, int y) override;
    void getDistancesFromEnds(juce::Point<float> p, double& distanceFromStart, double& distanceFromEnd);

private:
    DspEditor* editor_;
    juce::Point<float> lastInputPos_;
    juce::Point<float> lastOutputPos_;
    Pin* source_;
    Pin* destination_;
    juce::Path path_;
    juce::Path hitPath_;
    bool dragging_;
};
