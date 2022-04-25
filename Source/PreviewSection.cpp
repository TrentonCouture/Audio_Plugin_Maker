/*
  ==============================================================================

    PreviewSection.cpp
    Created: 20 Feb 2021 1:48:41pm
    Author:  Trent

  ==============================================================================
*/

#include "PreviewSection.h"

PreviewSection::PreviewSection(DspEditor& editor, GuiEditor& guiEditor) : editor_(editor), guiEditor_(guiEditor)
{
    addAndMakeVisible(previewButton_);
    previewButton_.setButtonText("Preview");
    previewButton_.addListener(this);
}

void PreviewSection::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::deepskyblue);
}

void PreviewSection::resized()
{
    previewButton_.setBounds(getLocalBounds());
}

void PreviewSection::buttonClicked(juce::Button* button)
{
    if (button == &previewButton_)
    {
        juce::OwnedArray<Processor>& processors = editor_.getProcessors();
        juce::OwnedArray<Connector>& connectors = editor_.getConnectors();
        
        auto state = setState(processors, connectors);
        exportJSON(state, "C:\\Users\\Trent\\AppData\\dsp.json");
        guiEditor_.exportJSON("C:\\Users\\Trent\\AppData\\gui.json");
    }
}

juce::var PreviewSection::setState(juce::OwnedArray<Processor>& processors, juce::OwnedArray<Connector>& connectors)
{
    juce::var state(new juce::DynamicObject);
    for (auto& processor : processors)
    {
        auto processorVal = new juce::DynamicObject();
        processorVal->setProperty("type", processor->getType());


        auto ins = std::make_unique<juce::Array<juce::var>>();
        auto outs = std::make_unique<juce::Array<juce::var>>();

        for (int i = 0; i < processor->getNumAudioIn(); i++)
            ins->add(juce::Array<juce::var>());
        for (int i = 0; i < processor->getNumAudioOut(); i++)
            outs->add(juce::Array<juce::var>());

        if (processor->getType() == "Input")
        {
            ins->add(juce::Array<juce::var>());
            (*ins)[0].append("in");
        }
        if (processor->getType() == "Output")
        {
            outs->add(juce::Array<juce::var>());
            (*outs)[0].append("out");
        }

        for (auto& connector : connectors)
        {
            if (connector->getSource()->getProcessor() == processor
                && !connector->getSource()->isMidi())
            {
                (*outs)[connector->getSource()->getPinNum()].append(connector->getDestination()->getProcessor()->getID() + ":" +
                    juce::String(connector->getDestination()->getPinNum() + 1));
            }
            else if (connector->getDestination()->getProcessor() == processor
                && !connector->getDestination()->isMidi())
            {
                (*ins)[connector->getDestination()->getPinNum()].append(connector->getSource()->getProcessor()->getID() + ":" +
                    juce::String(connector->getSource()->getPinNum() + 1));
            }
        }

        processorVal->setProperty("in", *ins);
        processorVal->setProperty("out", *outs);

        auto midiIns = std::make_unique<juce::Array<juce::var>>();
        auto midiOuts = std::make_unique<juce::Array<juce::var>>();

        for (int i = 0; i < processor->getNumMidiInputPins(); i++)
            midiIns->add(juce::Array<juce::var>());
        for (int i = 0; i < processor->getNumMidiOutputPins(); i++)
            midiOuts->add(juce::Array<juce::var>());

        for (auto& connector : connectors)
        {
            if (connector->getSource()->getProcessor() == processor
                && connector->getSource()->isMidi())
            {
                (*midiOuts)[connector->getSource()->getPinNum() - connector->getSource()->getProcessor()->getNumAudioOut()]
                    .append(connector->getDestination()->getProcessor()->getID() + ":" + 
                    juce::String(connector->getDestination()->getPinNum() - connector->getDestination()->getProcessor()->getNumAudioIn() + 1));
            }
            else if (connector->getDestination()->getProcessor() == processor
                && connector->getDestination()->isMidi())
            {
                (*midiIns)[connector->getDestination()->getPinNum() - connector->getDestination()->getProcessor()->getNumAudioIn()]
                    .append(connector->getSource()->getProcessor()->getID());
            }
        }

        processorVal->setProperty("midiIn", *midiIns);
        processorVal->setProperty("midiOut", *midiOuts);

        for (auto& parameter : processor->getParameters())
            processorVal->setProperty(parameter.name, parameter.value);

        auto statePtr = state.getDynamicObject();
        statePtr->setProperty(processor->getID(), processorVal);
    }

    return state;

}

void PreviewSection::exportJSON(juce::var state, std::string filepath)
{
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
