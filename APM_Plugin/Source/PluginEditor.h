/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class APM_PluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    APM_PluginAudioProcessorEditor (APM_PluginAudioProcessor&);
    ~APM_PluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    APM_PluginAudioProcessor& audioProcessor;

    juce::var guiJSON_;
    juce::OwnedArray<juce::Slider> sliders_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (APM_PluginAudioProcessorEditor)
};
