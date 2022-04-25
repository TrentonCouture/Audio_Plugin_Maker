/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
APM_PluginAudioProcessorEditor::APM_PluginAudioProcessorEditor (APM_PluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 550);

    juce::File dspFile("C:\\Users\\Trent\\AppData\\gui.json");
    guiJSON_ = juce::JSON::parse(dspFile);

    if (auto* componentList = guiJSON_.getDynamicObject())
    {
        for (auto& component : componentList->getProperties())
        {
            if (component.value["type"] == "Slider")
            {
                sliders_.add(new juce::Slider);
                addAndMakeVisible(sliders_.getLast());
                addChildAndSetID(sliders_.getLast(), component.name.toString());
                sliders_.getLast()->setSliderStyle(juce::Slider::RotaryVerticalDrag);
	            sliders_.getLast()->setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
                sliders_.getLast()->setRange(0.0, 1.0);
                resized();
            }
        }
    }
}

APM_PluginAudioProcessorEditor::~APM_PluginAudioProcessorEditor()
{
}

//==============================================================================
void APM_PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("This is an auto-generated VST!", getLocalBounds(), juce::Justification::centred, 1);
}

void APM_PluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    for (auto& slider : sliders_)
        if (auto* componentList = guiJSON_.getDynamicObject())
            for (auto& component : componentList->getProperties())
                if (component.name.toString() == slider->getComponentID())
                    slider->setBoundsRelative((float)component.value["x"], (float)component.value["y"], 0.15, 0.15);
}
