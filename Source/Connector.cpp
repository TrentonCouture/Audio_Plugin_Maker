/*
  ==============================================================================

    Connector.cpp
    Created: 19 Feb 2021 6:55:56pm
    Author:  Trent

  ==============================================================================
*/

#include "Connector.h"
#include "DspEditor.h"

Connector::Connector(DspEditor* editor) : editor_(editor), source_(nullptr), destination_(nullptr), dragging_(false)
{
    lastInputPos_ = juce::Point<float>(0, 0);
    lastOutputPos_ = juce::Point<float>(0, 0);
}

void Connector::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.fillPath(path_);
}

void Connector::resized()
{
    juce::Point<float> p1, p2;
    getPoints(p1, p2);

    lastInputPos_ = p1;
    lastOutputPos_ = p2;

    p1 -= getPosition().toFloat();
    p2 -= getPosition().toFloat();

    path_.clear();
    path_.startNewSubPath (p1);
    path_.lineTo(p2.x, p2.y);

    juce::PathStrokeType wideStroke (8.0f);
    wideStroke.createStrokedPath (hitPath_, path_);


    juce::PathStrokeType stroke (2.5f);
    stroke.createStrokedPath (path_, path_);
    path_.setUsingNonZeroWinding (true);
}

void Connector::resizedToFit()
{
    juce::Point<float> p1, p2;
    getPoints (p1, p2);

    auto newBounds = juce::Rectangle<float> (p1, p2).expanded (4.0f).getSmallestIntegerContainer();

    if (newBounds != getBounds())
        setBounds (newBounds);
    else
        resized();

    repaint();
}

void Connector::mouseUp(const juce::MouseEvent& e)
{
    editor_->endDraggingConnector(e);
}

void Connector::mouseDown(const juce::MouseEvent& e)
{
    dragging_ = false;
}

void Connector::mouseDrag(const juce::MouseEvent& e)
{
    if (dragging_)
    {
        editor_->dragConnector(e);
    }
    else
    {
        dragging_ = true;

        double distanceFromStart, distanceFromEnd;
        getDistancesFromEnds (getPosition().toFloat() + e.position, distanceFromStart, distanceFromEnd);
        const bool isNearerSource = (distanceFromStart < distanceFromEnd);

        if (isNearerSource)
            source_ = nullptr;
        else
            destination_ = nullptr;

        editor_->startConnectorDrag (isNearerSource ? nullptr : source_,
                                  isNearerSource ? destination_ : nullptr,
                                  e);
    }

}

void Connector::getPoints(juce::Point<float>& p1, juce::Point<float>& p2)
{
    p1 = lastInputPos_;
    p2 = lastOutputPos_;

    if (source_)
        p1 = source_->getProcessor()->getPinPoint(source_->getPinNum(), false);
    if (destination_)
        p2 = destination_->getProcessor()->getPinPoint(destination_->getPinNum(), true);
}

void Connector::setSource(Pin* source)
{
    source_ = source;
}
void Connector::setDestination(Pin* destination)
{
    destination_ = destination;
}

Pin* Connector::getSource()
{
    return source_;
}

Pin* Connector::getDestination()
{
    return destination_;
}

void Connector::dragStart(juce::Point<float> pos)
{
    lastOutputPos_ = pos;
    resizedToFit();
}

void Connector::dragEnd(juce::Point<float> pos)
{
    lastInputPos_ = pos;
    resizedToFit();
}

void Connector::update()
{
    juce::Point<float> p1, p2;
    getPoints (p1, p2);

    if (lastInputPos_ != p1 || lastOutputPos_ != p2)
        resizedToFit();

}

bool Connector::hitTest(int x, int y)
{
    auto pos = juce::Point<int> (x, y).toFloat();
 
    if (hitPath_.contains (pos))
    {
        double distanceFromStart, distanceFromEnd;
        getDistancesFromEnds (pos, distanceFromStart, distanceFromEnd);
 
        // avoid clicking the connector when over a pin
        return distanceFromStart > 7.0 && distanceFromEnd > 7.0;
    }
 
    return false;

}
     
void Connector::getDistancesFromEnds(juce::Point<float> p, double& distanceFromStart, double& distanceFromEnd)
{
    juce::Point<float> p1, p2;
    getPoints(p1, p2);

    distanceFromStart = p1.getDistanceFrom(p);
    distanceFromEnd = p2.getDistanceFrom(p);
}