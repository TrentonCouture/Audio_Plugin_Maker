/*
  ==============================================================================

    GuiEditor.cpp
    Created: 24 Mar 2021 6:29:57pm
    Author:  Trent

  ==============================================================================
*/

#include "GuiEditor.h"

GuiEditor::GuiEditor() : newId_(1) {}

void GuiEditor::paint(juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //g.fillAll (juce::Colours::fuchsia);
}

void GuiEditor::resized() 
{
}

void GuiEditor::mouseDown(const juce::MouseEvent& e)
{
    if (e.mods.isPopupMenu())
        makePopupMenu(e.position.toInt());
}

void GuiEditor::makePopupMenu(juce::Point<int> position)
{
    popupMenu_.reset(new juce::PopupMenu);

    popupMenu_->addItem(1, "Slider");
    popupMenu_->addItem(2, "Button");

    popupMenu_->showMenuAsync({}, juce::ModalCallbackFunction::create([this, position](int choice)
        {
            if (choice > 0)
            {
                switch (choice)
                {
                case 1:
                    sliders_.add(new MovableSlider(juce::String(newId_++)));
                    addAndMakeVisible(sliders_.getLast());
                    sliders_.getLast()->setSliderStyle(MovableSlider::RotaryVerticalDrag);
	                sliders_.getLast()->setTextBoxStyle(MovableSlider::NoTextBox, true, 0, 0);
                    sliders_.getLast()->setRange(0.0, 1.0);
                    sliders_.getLast()->setBoundsRelative(sliders_.getLast()->getPosition().x, sliders_.getLast()->getPosition().y, 0.15, 0.15);
                    sliders_.getLast()->setCentrePosition(position);
                    break;
                case 2:
                    buttons_.add(new MovableButton(juce::String(newId_++)));
                    addAndMakeVisible(buttons_.getLast());
                    buttons_.getLast()->setBoundsRelative(buttons_.getLast()->getPosition().x, buttons_.getLast()->getPosition().y, 0.10, 0.05);
                    buttons_.getLast()->setCentrePosition(position);
                    break;
                }
            }
        }));
}

juce::var GuiEditor::setState()
{
    juce::var state(new juce::DynamicObject);

    for (auto& slider : sliders_)
    {
        auto sliderVal = new juce::DynamicObject();
        sliderVal->setProperty("type", "Slider");
        sliderVal->setProperty("x", slider->getPosition().x);
        sliderVal->setProperty("y", slider->getPosition().y);

        auto statePtr = state.getDynamicObject();
        statePtr->setProperty(slider->getID(), sliderVal);
    }

    return state;
}

void GuiEditor::exportJSON(juce::String filepath)
{
    auto state = setState();

    juce::File outFile(filepath);
    auto result = outFile.create();
    DBG(result.getErrorMessage());

    if (!outFile.existsAsFile())
        DBG("file doesn't exist");

    juce::FileOutputStream stream (outFile);

 
    if (stream.openedOk())
    {
        stream.setPosition (0);
        stream.truncate();

        juce::JSON::writeToStream(stream, state);
        stream.flush();
    }


}
