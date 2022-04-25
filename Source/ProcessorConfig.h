/*
  ==============================================================================

    ProcessorConfig.h
    Created: 6 Mar 2021 1:34:56pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct ProcessorConfig
{
    // make this a class and use encapsulation

    struct ProcessorTypes
    {
        std::string name_;
        std::vector<int> io_;
        std::vector<std::pair<std::string, float>> parameters_;
    };

    std::vector<ProcessorTypes> types =
    {
        {"Input", {0, 0, 0, 1}, {}},
        {"Output", {0, 0, 1, 0}, {}},
        {"Oscillator", {1, 0, 1, 1}, {{"freq", 440}}},
        {"Gain", {0, 0, 1, 1}, {}},
        {"Filter", {2, 0, 1, 1}, {{"cutoff", 1000},{"resonance", 0.5}}},
        {"MidiSine", {0, 1, 0, 0}, {{"frequency", 50.0}}},
        {"MidiSaw", {0, 1, 0, 0}, {{"frequency", 50.0}}},
        {"Convolution", {0, 0, 2, 1}, {}},
        {"RMS", {0, 1, 1, 0}, {}}
    };

    int numMidiIn_ = 0;
    int numMidiOut_ = 0;
    int numAudioIn_ = 0;
    int numAudioOut_ = 0;
    juce::DynamicObject parameters_;


    ProcessorConfig(int choice)
    {
        choice += 1;
        numMidiIn_ = types.at(choice).io_.at(0);
        numMidiOut_ = types.at(choice).io_.at(1);
        numAudioIn_ = types.at(choice).io_.at(2);
        numAudioOut_ = types.at(choice).io_.at(3);

        for (auto& param : types.at(choice).parameters_)
            parameters_.setProperty(juce::String(param.first), param.second);
    }
};
