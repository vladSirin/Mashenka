#pragma once
#include <glm/glm.hpp>
#include "SceneCamera.h"
#include "ScriptEntity.h"
#include "glm/gtx/transform.hpp"

/* This is the component class for ECS system, utilizing ENTT module
 * Setup the basic components for transform and SpriteRender
 */
namespace Mashenka
{
	// Tag component is used to label entities with a string tag=
	struct TagComponent
	{
		std::string Tag; // the tag

		TagComponent() = default;
		TagComponent(const TagComponent&) = default; // copy
		TagComponent(const std::string& tag) // parameterized constructor
			: Tag(tag)
		{
		}
	};


	struct TransformComponent
	{
		// Refactor the transform matrix into Translation, Rotation and Scale
		glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
		glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
		glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default; // Default copy constructor, shallow copy

		/* @Shallow Copy: Copies all fields of the original object, including references.
		 * Both objects end up sharing the same referenced objects.
		 * @Deep Copy: Copies all fields and creates copies of any referenced objects as well.
		 * This way, the new object has its own copies of the objects that the original object referenced.*/

		TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
			: Translation(translation), Rotation(rotation), Scale(scale)
		{
		}

		// Calculate and return the transform matrix by translate * rotation * scale
		glm::mat4 GetTransform() const
		{
			// Consider encapsulating this logic
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, {1, 0, 0})
				* glm::rotate(glm::mat4(1.0f), Rotation.y, {0, 1, 0})
				* glm::rotate(glm::mat4(1.0f), Rotation.z, {0, 0, 1});

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
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

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; //TODO: think about moving to Scene
		bool FixedAspectRatio = false; // To control aspect ratio

		CameraComponent() = default; //using default as constructor will calculate projection
		CameraComponent(const CameraComponent&) = default;
	};


	/* @brief This ia a component class to support C++ scripting
	 * It can be bound to any ScriptEntity so the scripts can be invoked on Events
	 */
	struct NativeScriptComponent
	{
		ScriptEntity* Instance = nullptr;


		// Function pointers
		// Declare function ptr that points to a function returning and what arguments it takes
		// @Return ScriptEntity*
		// @Name InstantiateScript
		// @Param is None as it is ended with ()
		ScriptEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);


		// Assign the functions to lambda functions with related operations
		template <typename T>
		void Bind()
		{
			// Capture by value as they are just calling the method
			InstantiateScript = []() { return static_cast<ScriptEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc)
			{
				delete nsc->Instance;
				nsc->Instance = nullptr;
			};
		}
	};
}
