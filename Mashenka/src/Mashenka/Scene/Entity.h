#pragma once
#include "Scene.h"
#include "entt.hpp"

namespace Mashenka
{
    /**
     * @class Entity
     * @brief Represents an entity within a scene managed by the EnTT library.
     *
     * The Entity class encapsulates an entity handle and provides methods to add,
     * get, check, and remove components. It operates within the context of a Scene
     * and utilizes the EnTT registry for component management.
     */
    class Entity
    {
    public:
        /**
         * @brief Default constructor for the Entity class.
         *
         * Initializes an empty entity with a null handle and no associated scene.
         */
        Entity() = default;

        /**
         * @brief Parameterized constructor for the Entity class.
         *
         * Initializes an entity with the given handle and scene.
         * @param handle The EnTT entity handle.
         * @param scene Pointer to the scene managing this entity.
         */
        Entity(entt::entity handle, Scene* scene);

        /**
         * @brief Default copy constructor for the Entity class.
         *
         * Performs a shallow copy of the entity, copying the handle and scene pointer.
         * @param other The Entity object to copy from.
         */
        Entity(const Entity& other) = default;

        /**
         * @brief Adds a component to the entity.
         *
         * Template method to add a component of type T to the entity.
         * Asserts if the entity already has the component.
         * @tparam T The type of component to add.
         * @tparam Args The types of arguments to pass to the component's constructor.
         * @param args The arguments to pass to the component's constructor.
         * @return Reference to the newly added component.
         */
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) // Args&& perfect forwarding
        {
            MK_CORE_ASSERT(!HasComponent<T>(), "Entity already has the component!");
            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        /**
         * @brief Retrieves a component from the entity.
         *
         * Template method to get a component of type T from the entity.
         * Asserts if the entity does not have the component.
         * @tparam T The type of component to retrieve.
         * @return Reference to the component.
         */
        template<typename T>
        T& GetComponent()
        {
            MK_CORE_ASSERT(HasComponent<T>(), "Entity does not have the component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        /**
         * @brief Checks if the entity has a specific component.
         *
         * Template method to check if the entity has a component of type T.
         * @tparam T The type of component to check for.
         * @return True if the entity has the component, false otherwise.
         */
        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        /**
         * @brief Removes a component from the entity.
         *
         * Template method to remove a component of type T from the entity.
         * Asserts if the entity does not have the component.
         * @tparam T The type of component to remove.
         */
        template<typename T>
        void RemoveComponent()
        {
            MK_CORE_ASSERT(HasComponent<T>(), "Entity does not have the component!");
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        /**
         * @brief Conversion operator to bool.
         *
         * Allows the entity to be used in boolean expressions, returning true if the entity handle is valid.
         * @return True if the entity handle is not null, false otherwise.
         */
        operator bool() const { return m_EntityHandle != entt::null; }

    private:
        entt::entity m_EntityHandle{ entt::null }; /**< The EnTT entity handle. */
        Scene* m_Scene = nullptr; /**< Pointer to the scene managing this entity. */
    };
}
