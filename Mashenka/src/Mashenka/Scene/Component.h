#pragma once
#include <glm/glm.hpp>

/* This is the component class for ECS system, utilizing ENTT module
 * Setup the basic components for transform and SpriteRender
 */
namespace Mashenka
{
    struct TransformComponent
    {
        glm::mat4 Transform{1.0f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default; // Default copy constructor, shallow copy

        /* @Shallow Copy: Copies all fields of the original object, including references.
         * Both objects end up sharing the same referenced objects.
         * @Deep Copy: Copies all fields and creates copies of any referenced objects as well.
         * This way, the new object has its own copies of the objects that the original object referenced.*/
        
        TransformComponent(const glm::mat4& transform)
            : Transform(transform)
        {
        }

        // Conversion operators
        // converts a transformComponent to a non-const reference to its Transform matrix
        operator glm::mat4&() { return Transform; }
        operator const glm::mat4&() const { return Transform; } // converts to a const reference
    };

    struct SpriteRenderComponent
    {
        glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};

        SpriteRenderComponent() = default;
        SpriteRenderComponent(const SpriteRenderComponent&) = default;

        SpriteRenderComponent(const glm::vec4& color)
            : Color(color)
        {
        }
    };
}
