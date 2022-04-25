/*
  ==============================================================================

    Engine.cpp
    Created: 9 Feb 2021 7:20:57pm
    Author:  Trent

  ==============================================================================
*/

#include "Engine.h"

Engine::Engine() : mainProcessor_(new juce::AudioProcessorGraph()), sampleRate_(0), frameSize_(0), numChannels_(0), maxID_(0), maxChannels_(4) {}

void Engine::initialise(int sampleRate, int frameSize, int numChannels)
{
    sampleRate_ = sampleRate;
    frameSize_ = frameSize;
    numChannels_ = numChannels;
    juce::File dspFile("C:\\Users\\Trent\\AppData\\dsp.json");
    //juce::File dspFile("C:\\Users\\Trent\\Projects_cpp\\AudioPluginMaker\\APM_Plugin\\Source\\dsp_old.json");
    json_ = juce::JSON::parse(dspFile);



    mainProcessor_->setPlayConfigDetails (maxChannels_,
                                         maxChannels_,
                                         sampleRate, frameSize);

    mainProcessor_->prepareToPlay (sampleRate, frameSize);

    initialiseGraph();
    updateGraph(); // call this after you change var object
}

void Engine::process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    mainProcessor_->processBlock (buffer, midiMessages);
}

void Engine::releaseResources()
{
    mainProcessor_->releaseResources();
}

void Engine::initialiseGraph()
{
    mainProcessor_->clear();
}

void Engine::updateGraph()
{
    for (auto connection : mainProcessor_->getConnections())
        mainProcessor_->removeConnection (connection);

    maxID_ = getMaxID();

    if (auto* blockList = json_.getDynamicObject())
    {
        for (auto& prevBlock : blockList->getProperties())
            for (auto& block : blockList->getProperties())
                connectTwoNodes(prevBlock, block);
    }

    for (auto node : mainProcessor_->getNodes())
        node->getProcessor()->enableAllBuses();
}

int Engine::getMaxID()
{
    int max = 0;

    if (auto* blockList = json_.getDynamicObject())
    {
        for (auto& block : blockList->getProperties())
        {
            int id = std::stoi(block.name.toString().toStdString());
            if (id > max)
            {
                max = id;
            }
        }
    }

    return ++max;
}


std::vector<juce::AudioProcessorGraph::NodeAndChannel> Engine::addToGraph(juce::NamedValueSet::NamedValue block1, juce::NamedValueSet::NamedValue block2)
{
    std::vector<juce::AudioProcessorGraph::NodeAndChannel> nodeAndChannels;
    for (auto& aBlock : { block1, block2 })
    {
        auto processorId = aBlock.name.toString().toStdString();
        std::stringstream ss(processorId);
        juce::uint32 newId;
        ss >> newId;
        if (!mainProcessor_->getNodeForId(juce::AudioProcessorGraph::NodeID(newId)))
        {
            auto processor = processorFactory_.getProcessor(processorId, aBlock.value);
            processor->setPlayConfigDetails(maxChannels_, maxChannels_, sampleRate_, frameSize_);
            auto node = mainProcessor_->addNode(std::move(processor), juce::AudioProcessorGraph::NodeID(newId));
        }

        for (int channel = 0; channel < numChannels_; channel++)
        {
            juce::AudioProcessorGraph::NodeAndChannel nodeAndChannel;
            nodeAndChannel.nodeID = juce::AudioProcessorGraph::NodeID(newId);
            nodeAndChannel.channelIndex = channel;

            nodeAndChannels.push_back(nodeAndChannel);
        }

        juce::AudioProcessorGraph::NodeAndChannel nodeAndChannel;
        nodeAndChannel.nodeID = juce::AudioProcessorGraph::NodeID(newId);
        nodeAndChannel.channelIndex = juce::AudioProcessorGraph::midiChannelIndex;

        nodeAndChannels.push_back(nodeAndChannel);

    }

    return nodeAndChannels;
}

void Engine::connectAudio(juce::NamedValueSet::NamedValue prevBlock, juce::NamedValueSet::NamedValue block, std::vector<juce::AudioProcessorGraph::NodeAndChannel> nodeAndChannels)
{
    auto prevInputs = prevBlock.value["in"].getArray();
    auto prevOutputs = prevBlock.value["out"].getArray();
    auto curInputs = block.value["in"].getArray();
    auto curOutputs = block.value["out"].getArray();

    std::vector<int> sizes = { prevInputs->size(), prevOutputs->size(), curInputs->size(), curOutputs->size() };
    int maxPins = *std::max_element(sizes.begin(), sizes.end());

    for (int chan = 1; chan <= maxPins; chan++)
    { 
        for (int chan2 = 1; chan2 <= maxPins; chan2++)
        {
            for (int i = 0; i < prevInputs->size(); i++)
                for (int j = 0; j < curOutputs->size(); j++)
                    if (!prevInputs->isEmpty()
                        && !curOutputs->isEmpty()
                        && prevInputs[0][i].getArray()->contains(block.name.toString() + ":" + juce::String(chan))
                        && curOutputs[0][j].getArray()->contains(prevBlock.name.toString() + ":" + juce::String(chan2))
                        && !mainProcessor_->isConnected(juce::AudioProcessorGraph::Connection(nodeAndChannels.at(3), nodeAndChannels.at(0)))
                        && !mainProcessor_->isConnected(juce::AudioProcessorGraph::Connection(nodeAndChannels.at(4), nodeAndChannels.at(1))))
                    {
                        for (int channel = 0; channel < numChannels_; channel++)
                            mainProcessor_->addConnection({ { nodeAndChannels.at(3).nodeID,  (numChannels_ * chan) - numChannels_ + channel }, 
                                { nodeAndChannels.at(0).nodeID, (numChannels_ * chan2) - numChannels_ + channel } });
                    }
            for (int i = 0; i < prevOutputs->size(); i++)
                for (int j = 0; j < curInputs->size(); j++)
                    if (!prevOutputs->isEmpty()
                        && !curInputs->isEmpty()
                        && prevOutputs[0][i].getArray()->contains(block.name.toString() + ":" + juce::String(chan))
                        && curInputs[0][j].getArray()->contains(prevBlock.name.toString() + ":" + juce::String(chan2))
                        && !mainProcessor_->isConnected(juce::AudioProcessorGraph::Connection(nodeAndChannels.at(0), nodeAndChannels.at(3)))
                        && !mainProcessor_->isConnected(juce::AudioProcessorGraph::Connection(nodeAndChannels.at(1), nodeAndChannels.at(4))))
                    {
                        for (int channel = 0; channel < numChannels_; channel++)
                            mainProcessor_->addConnection({ { nodeAndChannels.at(0).nodeID, (numChannels_ * chan2) - numChannels_ + channel }, 
                                { nodeAndChannels.at(3).nodeID, (numChannels_ * chan) - numChannels_ + channel } });
                    }
        }
    }
}

void Engine::connectMidi(juce::NamedValueSet::NamedValue prevBlock, juce::NamedValueSet::NamedValue block, std::vector<juce::AudioProcessorGraph::NodeAndChannel> nodeAndChannels)
{
    auto prevMidiIn = prevBlock.value["midiIn"].getArray();
    auto prevMidiOut = prevBlock.value["midiOut"].getArray();

    auto curMidiIn = block.value["midiIn"].getArray();
    auto curMidiOut = block.value["midiOut"].getArray();

    int numMidiChannels = 16;
    for (int chan = 1; chan <= numMidiChannels; chan++)
    {
        for (int i = 0; i < prevMidiIn->size(); i++)
            for (int j = 0; j < curMidiOut->size(); j++)
                if (!prevMidiIn->isEmpty()
                    && !curMidiOut->isEmpty()
                    && prevMidiIn[0][i].getArray()->contains(block.name.toString())
                    && curMidiOut[0][j].getArray()->contains(prevBlock.name.toString() + ":" + juce::String(chan))
                    // this is now useless...
                    && !mainProcessor_->isConnected(juce::AudioProcessorGraph::Connection(nodeAndChannels.at(5), nodeAndChannels.at(2))))
                {
                    if (!mainProcessor_->getNodeForId(juce::AudioProcessorGraph::NodeID(maxID_)))
                    {
                        juce::var config(new juce::DynamicObject);

                        auto blockVal = new juce::DynamicObject;
                        blockVal->setProperty("type", "MidiChannelProcessor");
                        blockVal->setProperty("channel", chan);

                        auto processor = processorFactory_.getProcessor(juce::String(maxID_).toStdString(), blockVal);
                        processor->setPlayConfigDetails(numChannels_, numChannels_, sampleRate_, frameSize_);
                        auto node = mainProcessor_->addNode(std::move(processor), juce::AudioProcessorGraph::NodeID(maxID_));
                        maxID_++;

                        mainProcessor_->addConnection({ { nodeAndChannels.at(5).nodeID, juce::AudioProcessorGraph::midiChannelIndex },
                                                        { node->nodeID, juce::AudioProcessorGraph::midiChannelIndex } });
                        mainProcessor_->addConnection({ { node->nodeID, juce::AudioProcessorGraph::midiChannelIndex },
                                                        { nodeAndChannels.at(2).nodeID, juce::AudioProcessorGraph::midiChannelIndex } });

                    }
                }
        for (int i = 0; i < prevMidiOut->size(); i++)
            for (int j = 0; j < curMidiIn->size(); j++)
                if (!prevMidiOut->isEmpty()
                    && !curMidiIn->isEmpty()
                    && prevMidiOut[0][i].getArray()->contains(block.name.toString())
                    && curMidiIn[0][j].getArray()->contains(prevBlock.name.toString() + ":" + juce::String(chan))
                    // this is now useless...
                    && !mainProcessor_->isConnected(juce::AudioProcessorGraph::Connection(nodeAndChannels.at(2), nodeAndChannels.at(5))))
                {
                    if (!mainProcessor_->getNodeForId(juce::AudioProcessorGraph::NodeID(maxID_)))
                    {
                        juce::var config(new juce::DynamicObject);

                        auto blockVal = new juce::DynamicObject;
                        blockVal->setProperty("type", "MidiChannelProcessor");
                        blockVal->setProperty("channel", chan);

                        auto processor = processorFactory_.getProcessor(juce::String(maxID_).toStdString(), blockVal);
                        processor->setPlayConfigDetails(numChannels_, numChannels_, sampleRate_, frameSize_);
                        auto node = mainProcessor_->addNode(std::move(processor), juce::AudioProcessorGraph::NodeID(maxID_));
                        maxID_++;

                        mainProcessor_->addConnection({ { nodeAndChannels.at(2).nodeID, juce::AudioProcessorGraph::midiChannelIndex },
                                                        { node->nodeID, juce::AudioProcessorGraph::midiChannelIndex } });
                        mainProcessor_->addConnection({ { node->nodeID, juce::AudioProcessorGraph::midiChannelIndex },
                                                        { nodeAndChannels.at(5).nodeID, juce::AudioProcessorGraph::midiChannelIndex } });
                    }
                }
    }
}

void Engine::connectTwoNodes(juce::NamedValueSet::NamedValue prevBlock, juce::NamedValueSet::NamedValue block)
{
    auto nodeAndChannels = addToGraph(prevBlock, block);
    connectAudio(prevBlock, block, nodeAndChannels);
    connectMidi(prevBlock, block, nodeAndChannels);
}
