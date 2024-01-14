#include "mkpch.h"
#include "Mashenka/Core/LayerStack.h"

namespace Mashenka
{
    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_Layers)
        {
            layer->OnDetach(); // call OnDetach when the layer is deleted
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
        layer->OnAttach(); // call OnAttach when the layer is pushed into the stack
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        m_Layers.emplace_back(overlay);
        overlay->OnAttach();
    }
    
    void LayerStack::PopLayer(Layer* layer)
    {
        // only check the layers before the overlay
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if(it != m_Layers.begin() + m_LayerInsertIndex) // from begin to the layerInsertIndex is the layers
        {
            layer->OnDetach(); // call OnDetach when layer is poped
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        // only check the overlays after the layers
        auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end())
        {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    }






}
