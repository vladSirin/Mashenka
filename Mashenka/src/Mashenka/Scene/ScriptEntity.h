#pragma once

#include "Entity.h"

/* @brief A class that associate the entity with the script instance the component bound to*/
namespace Mashenka
{
    class ScriptEntity
    {
    public:
        // Getting the designated component from the associated entity
        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }

    private:
        Entity m_Entity;
        friend class Scene; // to access private members
    };

    
}
