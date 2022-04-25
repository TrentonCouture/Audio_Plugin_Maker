/*
  ==============================================================================

    Engine.h
    Created: 9 Feb 2021 7:20:57pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "AudioProcessorFactory.h"


class Engine
{
public:
    Engine();
    void initialise(int sampleRate, int frameSize, int numChannels);
    void process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void releaseResources();
private:
    //==============================================================================
    using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = juce::AudioProcessorGraph::Node;

    //==============================================================================
    void initialiseGraph();
    void updateGraph();
    std::vector<juce::AudioProcessorGraph::NodeAndChannel> addToGraph(juce::NamedValueSet::NamedValue block1, juce::NamedValueSet::NamedValue block2);
    void connectAudio(juce::NamedValueSet::NamedValue prevBlock, juce::NamedValueSet::NamedValue block, std::vector<juce::AudioProcessorGraph::NodeAndChannel> nodeAndChannels);
    void connectMidi(juce::NamedValueSet::NamedValue prevBlock, juce::NamedValueSet::NamedValue block, std::vector<juce::AudioProcessorGraph::NodeAndChannel> nodeAndChannels);
    void connectTwoNodes(juce::NamedValueSet::NamedValue prevBlock, juce::NamedValueSet::NamedValue block);
    int getMaxID();


    //==============================================================================
    std::unique_ptr<juce::AudioProcessorGraph> mainProcessor_;

    int sampleRate_;
    int frameSize_;
    int numChannels_;
    int maxChannels_;

    int maxID_;

    juce::var json_;
    AudioProcessorFactory processorFactory_;

    juce::OwnedArray<juce::AudioProcessorGraph::Node::Ptr> midiChannelNodes_;


};
