/*
  ==============================================================================

    DspEditor.cpp
    Created: 15 Feb 2021 12:29:19pm
    Author:  Trent

  ==============================================================================
*/

#include "DspEditor.h"

DspEditor::DspEditor() : newId_(5), processorConfig_(0)
{
    processors_.add(new Processor(*this, "Input", juce::String(3), ProcessorConfig(-1)));
    addAndMakeVisible(processors_.getLast());
    processors_.getLast()->setPosition(0.5, 0.1);

    processors_.add(new Processor(*this, "Output", juce::String(4), ProcessorConfig(0)));
    addAndMakeVisible(processors_.getLast());
    processors_.getLast()->setPosition(0.5, 0.9);
}

void DspEditor::paint(juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void DspEditor::resized()
{
    for (auto& processor : processors_)
        processor->resized();
    for (auto& connector : connectors_)
    {
        connector->resizedToFit();
    }
}

void DspEditor::mouseDown(const juce::MouseEvent& e)
{

    if (e.mods.isPopupMenu())
        makePopupMenu(e.position.toInt());
}

void DspEditor::makePopupMenu(juce::Point<int> position)
{
    popupMenu_.reset(new juce::PopupMenu);

    for (int i = 1; i < processorConfig_.types.size() - 1; i++)
        popupMenu_->addItem(i, processorConfig_.types.at(i+1).name_);

    popupMenu_->showMenuAsync({}, juce::ModalCallbackFunction::create([this, position](int choice)
        {
            if (choice > 0)
            {
                processors_.add(new Processor(*this, processorConfig_.types.at(choice+1).name_, juce::String(newId_++), ProcessorConfig(choice)));
                addAndMakeVisible(processors_.getLast());
                processors_.getLast()->setCentrePosition(position);
            }
        }));
}


void DspEditor::deleteProcessors()
{
    for (int i = processors_.size() - 1; i >= 0; i--)
        if (processors_[i]->shouldBeDeleted())
        {
            for (auto& connector : connectors_)
            {
                if (connector->getSource()->getProcessor() == processors_[i])
                    connectors_.removeObject(connector);
                else if (connector->getDestination()->getProcessor() == processors_[i])
                    connectors_.removeObject(connector);
            }
            processors_.remove(i);
        }

    repaint();
}

void DspEditor::startConnectorDrag(Pin* source, Pin* destination, const juce::MouseEvent& e)
{
    auto* c = dynamic_cast<Connector*> (e.originalComponent);
    connectors_.removeObject (c, false);
    currentConnector_.reset (c);


    if (currentConnector_ == nullptr)
        currentConnector_.reset(new Connector(this));

    if (source)
        currentConnector_->setSource(source);
    else if (destination)
        currentConnector_->setDestination(destination);

    addAndMakeVisible(currentConnector_.get());
    currentConnector_->toFront(false);
    
    dragConnector(e);
}


void DspEditor::dragConnector(const juce::MouseEvent& e)
{
    auto pos = e.getEventRelativeTo(this).position;
    if (currentConnector_->getSource())
        currentConnector_->dragStart(pos);
    else if (currentConnector_->getDestination())
        currentConnector_->dragEnd(pos);

}

void DspEditor::endDraggingConnector(const juce::MouseEvent& e)
{
    if (currentConnector_ == nullptr)
        return;

    auto pos = e.getEventRelativeTo(this).position;

    if (auto pin = findPinAt(pos))
    {
        if (currentConnector_->getSource() 
            && currentConnector_->getSource()->getProcessor() != pin->getProcessor()
            && currentConnector_->getSource()->isMidi() == pin->isMidi()
            && currentConnector_->getSource()->isInput() != pin->isInput())
        {
            bool connect = true;
            for (auto& connector : connectors_)
                if (connector->getSource() == currentConnector_->getSource()
                    && connector->getDestination() == pin)
                    connect = false;

            if (connect)
            {
                currentConnector_->setDestination(pin);
                currentConnector_->resizedToFit();
                connectors_.add(std::move(currentConnector_));
            }
        }
        else if (currentConnector_->getDestination() 
            && currentConnector_->getDestination()->getProcessor() != pin->getProcessor()
            && currentConnector_->getDestination()->isMidi() == pin->isMidi()
            && currentConnector_->getDestination()->isInput() != pin->isInput())
        {
            bool connect = true;
            for (auto& connector : connectors_)
                if (connector->getDestination() == currentConnector_->getDestination()
                    && connector->getSource() == pin)
                    connect = false;
            if (connect)
            {
                currentConnector_->setSource(pin);
                currentConnector_->resizedToFit();
                connectors_.add(std::move(currentConnector_));
            }
        }

    }

    currentConnector_ = nullptr;
}

Pin* DspEditor::findPinAt(juce::Point<float> pos)
{
    for (auto* processor : processors_)
    {
        // NB: A Visual Studio optimiser error means we have to put this Component* in a local
        // variable before trying to cast it, or it gets mysteriously optimised away..
        auto* comp = processor->getComponentAt (pos.toInt() - processor->getPosition());

        if (auto* pin = dynamic_cast<Pin*> (comp))
            return pin;
    }

    return nullptr;
}


void DspEditor::update()
{
    for (auto connector : connectors_)
    {
        connector->update();
    }
}

Processor* DspEditor::getProcessorFromID(juce::String id)
{
    for (auto& processor : processors_)
        if (processor->getID() == id)
            return processor;

    return nullptr;
}


void DspEditor::deleteConnector(Connector* connector)
{
    connectors_.removeObject(connector);
}

void DspEditor::showParameterWindow(Processor& processor, juce::Point<int> position)
{
    paramWindow_.reset(new ParameterWindow(*this, 
                                           processor, 
                                           "Parameters", 
                                           juce::Colours::azure, 
                                           juce::DocumentWindow::TitleBarButtons::maximiseButton | juce::DocumentWindow::TitleBarButtons::closeButton, false));
    addAndMakeVisible(paramWindow_.get());

    int windowSize = 200;
    paramWindow_->setBounds(position.x, position.y, windowSize, windowSize);
    paramWindow_->setResizable(true, false);
}

void DspEditor::deleteParamWindow()
{
    paramWindow_.reset();
}

juce::OwnedArray<Processor>& DspEditor::getProcessors()
{
    return processors_;
}
juce::OwnedArray<Connector>& DspEditor::getConnectors()
{
    return connectors_;
}
