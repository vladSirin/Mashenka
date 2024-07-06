#pragma once

#include "entt.hpp"
#include "Mashenka/Core/TimeStep.h"

namespace Mashenka
{
    /**
     * @class Scene
     * @brief Manages a collection of entities and their components using the EnTT library.
     *
     * The Scene class encapsulates the creation, updating, and management of entities
     * and their associated components within a scene. It utilizes the EnTT library for
     * efficient entity-component-system (ECS) management.
     */
    class Scene
    {
    public:
        /**
         * @brief Constructs a new Scene object.
         *
         * Initializes the scene, setting up the internal registry for managing entities and components.
         */
        Scene();

        /**
         * @brief Destroys the Scene object.
         *
         * Cleans up any resources used by the scene, including the internal registry.
         */
        ~Scene();

        /**
         * @brief Creates a new entity within the scene.
         *
         * @return The handle to the newly created entity.
         */
        entt::entity CreateEntity();

        /**
         * @brief TEMP: Provides access to the internal registry.
         *
         * @return A reference to the internal EnTT registry.
         */
        entt::registry& Reg() { return m_Registry; }

        /**
         * @brief Updates the scene based on the elapsed time.
         *
         * This method is called every frame to update the state of the scene and its entities.
         * @param ts The time step representing the elapsed time since the last update.
         */
        void OnUpdate(TimeStep ts);

    private:
        entt::registry m_Registry; /**< The internal registry for managing entities and components. */
    };
}

