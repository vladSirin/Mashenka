﻿#pragma once
#include <glm/glm.hpp>
#include "SceneCamera.h"
#include "ScriptEntity.h"

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
