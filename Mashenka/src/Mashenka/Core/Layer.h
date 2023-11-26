#pragma once

#include "Timestep.h"
#include "Core.h"
#include "Mashenka/Events/Event.h"

namespace Mashenka
{
    /*The Layer class serves as a base class for different layers in the Mashenka game engine.
     *Layers can be thought of as individual components or stages in the rendering or update process of a game or application.
     *By providing virtual functions like OnAttach, OnDetach, OnUpdate, and OnEvent,
     *this design allows derived classes to implement custom behavior for these operations.*/
    class MASHENKA_API Layer
    {
    public:
        Layer(const std::string& name="Layer");
        virtual ~Layer() = default;

        virtual void OnAttach(){}
        virtual void OnDetach(){}
        virtual void OnUpdate(TimeStep ts){}
        virtual void OnImGuiRender(){} // Every layer could have its own thing to render
        virtual void OnEvent(Event& event){}

        inline const std::string& GetName() const {return m_DebugName;}

    protected:
        std::string m_DebugName;
        
    };
}

