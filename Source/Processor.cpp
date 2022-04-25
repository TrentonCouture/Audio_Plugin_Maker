/*
  ==============================================================================

    ProcessorComponent.cpp
    Created: 15 Feb 2021 3:54:43pm
    Author:  Trent

  ==============================================================================
*/

#include "Processor.h"
#include "DspEditor.h"

Processor::Processor(DspEditor& editor, juce::String type, juce::String id, ProcessorConfig config) 
    : editor_(editor), type_(type), id_(id), deleteMe_(false), numMidiIn_(0), numMidiOut_(0),
    numAudioIn_(0), numAudioOut_(0)
{

    numMidiIn_ = config.numMidiIn_;
    numMidiOut_ = config.numMidiOut_;
    numAudioIn_ = config.numAudioIn_;
    numAudioOut_ = config.numAudioOut_;
    parameters_ = config.parameters_;
    
    position_.x = 0.5f;
    position_.y = 0.5f;

    for (int i = 0; i < numAudioIn_; i++)
        pins.add(new Pin(editor_, *this, true, false, i));
    for (int i = 0; i < numAudioOut_; i++)
        pins.add(new Pin(editor_, *this, false, false, i));

    for (int i = numAudioIn_; i < numMidiIn_ + numAudioIn_; i++)
        pins.add(new Pin(editor_, *this, true, true, i));

    for (int i = numAudioOut_; i < numMidiOut_ + + numAudioOut_; i++)
        pins.add(new Pin(editor_, *this, false, true, i));

    for (auto& pin : pins)
        addAndMakeVisible(pin);
    
    setSize(75, 50);

}

void Processor::paint(juce::Graphics& g)
{
    auto boxArea = getLocalBounds().reduced (4, pinHeight_);
    auto reducedArea = boxArea.reduced(2, 2);

    g.setColour(juce::Colours::white);
    g.fillRect(boxArea);
    g.setColour(juce::Colours::black);
    g.fillRect(reducedArea);
    g.setColour(juce::Colours::white);
    g.drawText(type_, getLocalBounds(), juce::Justification::centred);
}

void Processor::resized()
{
    setCentreRelative(position_.x, position_.y);
    int inCount = 0;
    int outCount = 0;
    for (int i = 0; i < pins.size(); i++)
    {
        int x = 0;
        if (pins[i]->isInput())
        {
            x = getWidth() * (inCount + 1) / (numMidiIn_ + numAudioIn_ + 1) - pinWidth_ / 2;
            inCount++;
        }
        else
        {
            x = getWidth() * (outCount + 1) / (numMidiOut_ + numAudioOut_ + 1) - pinWidth_ / 2;
            outCount++;
        }
        pins[i]->setBounds(x, pins[i]->isInput() ? 0 : getHeight() - pinHeight_, pinWidth_, pinHeight_);
    }
}

void Processor::mouseDown(const juce::MouseEvent& e)
{
    originalPos_ = localPointToGlobal(juce::Point<int>());

    if (e.mods.isPopupMenu())
        makePopupMenu(e.position.toInt());
}

void Processor::mouseDrag(const juce::MouseEvent& e)
{
    auto pos = originalPos_ + e.getOffsetFromDragStart();

    pos = getParentComponent()->getLocalPoint (nullptr, pos);
    pos += getLocalBounds().getCentre();

    position_.x = pos.x / (float)getParentWidth();
    position_.y = pos.y / (float)getParentHeight();

    resized();

    editor_.update();
}

void Processor::makePopupMenu(juce::Point<int> position)
{
    popupMenu_.reset(new juce::PopupMenu);
    popupMenu_->addItem(1, "Set Parameters");
    popupMenu_->addItem(2, "Delete");

    popupMenu_->showMenuAsync({}, juce::ModalCallbackFunction::create([this, position](int choice)
        {
            if (choice > 0)
            {
                switch (choice)
                {
                case 1:
                    editor_.showParameterWindow(*this, juce::Point<int>(getRight(), getBottom()));
                    break;
                case 2:
                    deleteMe_ = true;
                    editor_.deleteProcessors();
                    break;
                }
                
            }
        }));
}

juce::Point<float> Processor::getPinPoint(int index, bool isInput)
{
    return juce::Point<float>(
        isInput ? getX() + getWidth() * (index + 1) / (numMidiIn_ + (numAudioIn_ +  1)) : 
        getX() + getWidth() * (index + 1) / (numMidiOut_ + (numAudioOut_ + 1)),
        isInput ? getY() + pinHeight_ / 2 : getY() + getHeight() - pinHeight_ / 2);
}

juce::String Processor::getID()
{
    return id_;
}

bool Processor::shouldBeDeleted()
{
    return deleteMe_;

}

juce::String Processor::getType()
{
    return type_;
}

bool Processor::hitTest(int x, int y)
{
    for (auto* child : getChildren())
        if (child->getBounds().contains (x, y))
            return true;

    return x >= 3 && x < getWidth() - 6 && y >= pinHeight_ && y < getHeight() - pinHeight_;
}

void Processor::setPosition(float x, float y)
{
    position_.x = x;
    position_.y = y;
}

juce::NamedValueSet Processor::getParameters()
{
    return parameters_.getProperties();
}

void Processor::setParameter(juce::String name, float value)
{
    parameters_.setProperty(name, value);
}

int Processor::getNumMidiInputPins()
{
    return numMidiIn_;
}

int Processor::getNumMidiOutputPins()
{
    return numMidiOut_;
}

int Processor::getNumAudioIn()
{
    return numAudioIn_;
}

int Processor::getNumAudioOut()
{
    return numAudioOut_;
}
