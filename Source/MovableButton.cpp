/*
  ==============================================================================

    MovableButton.cpp
    Created: 14 Apr 2021 6:25:39pm
    Author:  Trent

  ==============================================================================
*/

#include "MovableButton.h"

MovableButton::MovableButton(juce::String id) : ID_(id), ShapeButton(id, juce::Colours::black, juce::Colours::grey, juce::Colours::darkgrey)
{
    //addAndMakeVisible(button_);
    juce::Path buttonPath;
	buttonPath.addRectangle(0, 0, 15, 7);
	//button_.setShape(buttonPath, true, true, true);
	setShape(buttonPath, true, true, true);
}

void MovableButton::resized()
{
    //auto area = getLocalBounds();
	//button_.setBounds(area.removeFromLeft(button_.getWidth()));

    ShapeButton::resized();
    setCentreRelative(position_.x, position_.y);

    juce::Path buttonPath;
	buttonPath.addRectangle(getLocalBounds());
	setShape(buttonPath, false, true, true);
}

void MovableButton::mouseDown(const juce::MouseEvent& e)
{
    originalPos_ = localPointToGlobal(juce::Point<int>());

    if (e.mods.isPopupMenu())
        DBG("right clicked...");

}


void MovableButton::mouseDrag(const juce::MouseEvent& e)
{
    auto pos = originalPos_ + e.getOffsetFromDragStart();

    pos = getParentComponent()->getLocalPoint (nullptr, pos);
    pos += getLocalBounds().getCentre();

    position_.x = pos.x / (float)getParentWidth();
    position_.y = pos.y / (float)getParentHeight();

    resized();
}
