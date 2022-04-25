/*
  ==============================================================================

    DspEditor.h
    Created: 15 Feb 2021 12:29:19pm
    Author:  Trent

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "Processor.h"
#include "Connector.h"
#include "Pin.h"
#include "ParameterWindow.h"

class DspEditor : public juce::Component
{
public:
    DspEditor();
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;
    void makePopupMenu(juce::Point<int> position);
    void deleteProcessors();
    void deleteConnector(Connector* connector);
    void deleteParamWindow();
    void startConnectorDrag(Pin* source, Pin* destination, const juce::MouseEvent& e);
    void dragConnector(const juce::MouseEvent& e);
    void endDraggingConnector(const juce::MouseEvent& e);
    Processor* getProcessorFromID(juce::String id);
    void update();
    Pin* findPinAt(juce::Point<float> pos);
    void showParameterWindow(Processor& processor, juce::Point<int> position);
    juce::OwnedArray<Processor>& getProcessors();
    juce::OwnedArray<Connector>& getConnectors();

private:
    juce::OwnedArray<Processor> processors_;
    juce::OwnedArray<Connector> connectors_;
    std::unique_ptr<juce::PopupMenu> popupMenu_;
    std::unique_ptr<ParameterWindow> paramWindow_;
    std::unique_ptr<Connector> currentConnector_;
    ProcessorConfig processorConfig_;

    int newId_;
};
