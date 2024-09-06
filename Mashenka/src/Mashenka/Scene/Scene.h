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
    class Entity;

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
        Entity CreateEntity(const std::string& name = std::string());

    	void DestroyEntity(Entity entity);

        /**
         * @brief Updates the scene based on the elapsed time.
         *
         * This method is called every frame to update the state of the scene and its entities.
         * @param ts The time step representing the elapsed time since the last update.
         */
        void OnUpdate(TimeStep ts);

        void OnViewportResize(uint32_t width, uint32_t height);

    private:
    	template<typename  T>
    	void OnComponentAdded(Entity entity, T& component);

    private:
        entt::registry m_Registry; /**< The internal registry for managing entities and components. */
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity; /**< Grants scene the access to the private members of Entity class> */
    	friend class SceneHierarchyPanel;
    };
}
