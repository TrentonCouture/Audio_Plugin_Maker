/*
  ==============================================================================

    ParameterDisplay.cpp
    Created: 27 Feb 2021 8:16:07pm
    Author:  Trent

  ==============================================================================
*/

#include "ParameterDisplay.h"
#include "Processor.h"

ParameterDisplay::ParameterDisplay(Processor& processor) : processor_(processor) 
{
    for (auto& parameter : processor_.getParameters())
    {
        inputLabels_.add(new juce::Label());
        addAndMakeVisible(inputLabels_.getLast());
        inputLabels_.getLast()->setColour (juce::Label::backgroundColourId, juce::Colours::darkcyan);
        inputLabels_.getLast()->setEditable(true);
        auto thisLabel = inputLabels_.getLast();
        inputLabels_.getLast()->onTextChange = [this, parameter, thisLabel] { processor_.setParameter(parameter.name.toString(), std::stof(thisLabel->getText(false).toStdString())); };

        labels_.add(new juce::Label());
        addAndMakeVisible(labels_.getLast());
        labels_.getLast()->setText(parameter.name.toString(), juce::dontSendNotification);
        labels_.getLast()->setColour(juce::Label::textColourId, juce::Colours::darkcyan);
        labels_.getLast()->attachToComponent(inputLabels_.getLast(), true);

    }

    setSize(200, 200);
}

void ParameterDisplay::resized()
{
    int height = 20;
    int width = 50;
    for (int i = 0; i < labels_.size(); i++)
        labels_[i]->setBounds(0, i * height, width, height);
    for (int i = 0; i < inputLabels_.size(); i++)
        inputLabels_[i]->setBounds(width, i * height, width, height);
}
