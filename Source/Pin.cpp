/*
  ==============================================================================

    Pin.cpp
    Created: 18 Feb 2021 7:48:26pm
    Author:  Trent

  ==============================================================================
*/

#include "Pin.h"
#include "DspEditor.h"

Pin::Pin(DspEditor& editor, Processor& processor, bool isInput, bool isMidi, int pinNum) 
    : pinNum_(pinNum), isInput_(isInput), isMidi_(isMidi), editor_(editor), processor_(processor)
{
    setSize(16, 16);
}

void Pin::paint(juce::Graphics& g)
{
    //auto w = (float) getWidth();
    //auto h = (float) getHeight();

    //juce::Path p;
    //p.addEllipse (w * 0.25f, h * 0.25f, w * 0.5f, h * 0.5f);
    //p.addRectangle (w * 0.4f, isInput_ ? (0.5f * h) : 0.0f, w * 0.2f, h * 0.5f);


    auto colour = juce::Colours::white;
    if (isMidi_)
        colour = juce::Colours::grey;
    g.setColour (colour);
    g.fillRect(getLocalBounds());
    //g.fillPath (p);
}

void Pin::mouseDown(const juce::MouseEvent& e)
{
    editor_.startConnectorDrag(isInput_ ? nullptr : this, isInput_ ? this : nullptr, e);
}

void Pin::mouseDrag(const juce::MouseEvent& e)
{
    editor_.dragConnector(e);
}

void Pin::mouseUp(const juce::MouseEvent& e)
{
    editor_.endDraggingConnector(e);
}

Processor* Pin::getProcessor()
{
    return &processor_;
}

bool Pin::isInput()
{
    return isInput_;
}

int Pin::getPinNum()
{
    return pinNum_;
}

bool Pin::isMidi()
{
    return isMidi_;
}
