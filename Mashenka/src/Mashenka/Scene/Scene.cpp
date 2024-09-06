#include "mkpch.h"
#include "Scene.h"
#include "Entity.h"

#include "Component.h"
#include "Mashenka/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Mashenka
{
    /**
     * @brief Constructs a new Scene object.
     *
     * Initializes the scene, setting up the internal registry for managing entities and components.
     */
    Scene::Scene()
    {
        // Example code demonstrating various EnTT functionalities
#if ENTT_EXAMPLE_CODE

        // Define a temporary component for demonstration purposes
        struct MeshComponent
        {
            float a = 1;
        };

        // Create a new entity in the registry
        entt::entity entity = m_Registry.create();

        // Add a TransformComponent to the entity with an identity matrix
        m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

        // Connect a callback function to the construction of TransformComponent
        m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

        // Check if the entity has a TransformComponent and retrieve it
        if(m_Registry.all_of<TransformComponent>(entity))
            TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

        // Create a view for all entities with a TransformComponent and iterate over them
        auto view = m_Registry.view<TransformComponent>();
        for (auto entity : view)
        {
            TransformComponent& transform = view.get<TransformComponent>(entity);
        }

        // Create a group with entities that have both TransformComponent and MeshComponent
        // The main component, in this case TransformComponent will be stored in a contiguous array,
        // and the other components will be stored in separate contiguous arrays that are indexed in the same order
        // The 'Group' function leverage the fact that entities in the registry are essentially indices,
        // and components are stored in arrays (or vectors) indexed by these entity IDs.
        auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
        for (auto entity : group)
        {
            auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
        }

        /**
         * @brief EnTT architecture
         * Entity Storage: EnTT uses a sparse set to store entities, each entity is an index that maps to its components
         * Component Storage: Components are stored in arrays or vectors indexed by entity IDs. When you create a group,
         * EnTT can access these array efficiently
         * Contiguous Array: By creating a group, EnTT rearranges the internal storage such that entities with the
         * required components are placed in contiguous memory regions, this allows for efficient iteration and access
         *
         * This way, it avoids cache miss and reduce indirection accesss, simplifies the logic in game loops
         */

#endif
    }

    /**
     * @brief Destroys the Scene object.
     *
     * Cleans up any resources used by the scene, including the internal registry.
     */
    Scene::~Scene()
    {
        // Cleanup code (if any) goes here
    }

    /**
     * @brief Creates a new entity within the scene.
     * @param name for TagComponenet of the entity, if left empty, "Entity" will be used
     * @return The handle to the newly created entity.
     */
    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = {m_Registry.create(), this};
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
    	m_Registry.destroy(entity);
    }
    /**
     * @brief Updates the scene based on the elapsed time.
     *
     * This method is called every frame to update the state of the scene and its entities.
     * @param ts The time step representing the elapsed time since the last update.
     */
    void Scene::OnUpdate(TimeStep ts)
    {
        // Update all script components
        {
            // Iterate over all entities with a NativeScriptComponent
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
            {
                // If the script instance is not yet created
            	// TODO: move to Scene::OnScenePlay
                if (!nsc.Instance)
                {
                    // Instantiate the script instance
                    nsc.Instance = nsc.InstantiateScript();
                    // Associate the script instance with the entity
                    nsc.Instance->m_Entity = Entity{ entity, this };

                    // If there is an OnCreate function, call it
                    nsc.Instance->OnCreate();
                }

                // If there is an OnUpdate function, call it to update the script instance
                nsc.Instance->OnUpdate(ts);
            });
        }

        // Render 2D
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            // Iterate through transform and camera components to find primary and retrieve data
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity: view)
            {
                auto&& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }
            
        }

        // Render the scene based on the main Camera
        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, cameraTransform);

            auto view = m_Registry.group<TransformComponent>(entt::get<SpriteRenderComponent>);
            for (auto entity:view)
            {
                auto&& [transform, sprite] = view.get<TransformComponent, SpriteRenderComponent>(entity);
                Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
            }

            Renderer2D::EndScene();
        }
    }

    /* Called when a resize event happens, to resize the camera based on the new height/width*/
    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportHeight = height;
        m_ViewportWidth = width;

        // Resize our non-FixedAspectRatio cameras
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportSize(width, height);
            }
        }
    }

	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template <>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{}

	template <>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
	    component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }

	template <>
	void Scene::OnComponentAdded<SpriteRenderComponent>(Entity entity, SpriteRenderComponent& component)
    {
	    
    }

	template <>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {
	    
    }

	template <>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {
	    
    }
	

}
