/*
  ==============================================================================

    MovableSlider.cpp
    Created: 11 Apr 2021 3:08:25pm
    Author:  Trent

  ==============================================================================
*/

#include "MovableSlider.h"

MovableSlider::MovableSlider(juce::String id) : ID_(id) {}

void MovableSlider::resized()
{
    Slider::resized();

    setCentreRelative(position_.x, position_.y);
}

void MovableSlider::mouseDown(const juce::MouseEvent& e)
{
    originalPos_ = localPointToGlobal(juce::Point<int>());

    if (e.mods.isPopupMenu())
        DBG("right clicked...");
        //makePopupMenu(e.position.toInt());
}

void MovableSlider::mouseDrag(const juce::MouseEvent& e)
{
    auto pos = originalPos_ + e.getOffsetFromDragStart();

    pos = getParentComponent()->getLocalPoint (nullptr, pos);
    pos += getLocalBounds().getCentre();

    position_.x = pos.x / (float)getParentWidth();
    position_.y = pos.y / (float)getParentHeight();

    resized();

    //editor_.update();
}

juce::Point<float> MovableSlider::getPosition()
{
    return position_;
}

juce::String MovableSlider::getID()
{
    return ID_;
}
