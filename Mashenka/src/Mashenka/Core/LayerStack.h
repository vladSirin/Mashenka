﻿/*
 * The LayerStack class is a utility to manage layers in an application.
 * Layers can be thought of as individual "screens" or "panels" in an application.
 * For example, in a game, you might have a gameplay layer, a UI layer, and a debug info overlay.
 * By managing these in a stack, it allows for easy organization and
 * rendering in the correct order.
 * The distinction between layers and overlays can be thought of
 * as the difference between background content (layers) and foreground content (overlays).
 */
#pragma once

#include "Mashenka/Core/Base.h"
#include "Mashenka/Core/Layer.h"
#include <vector>

namespace Mashenka
{
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        // implement iterators and reverse iterators
        std::vector<Layer*>::iterator begin() {return m_Layers.begin();}
        std::vector<Layer*>::iterator end() {return  m_Layers.end();}
        std::vector<Layer*>::reverse_iterator rbegin() {return m_Layers.rbegin();}
        std::vector<Layer*>::reverse_iterator rend() {return m_Layers.rend();}
    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0; //using an index instead of iterator
    };
}
