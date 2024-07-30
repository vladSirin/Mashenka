#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include "Mashenka/Scene/Component.h"
#include <glm/gtc/type_ptr.hpp>

namespace Mashenka
{
	// Constructor with a context
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetConText(context);
	}

	// Set the scene context for the panel
	void SceneHierarchyPanel::SetConText(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	// Render the panel with ImGui
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		// Get the storage of all the entities
		const auto& storage = m_Context->m_Registry.storage<entt::entity>();

		// loop over all entities, create entity based on context and Draw them
		for (auto entityID : storage)
		{
			Entity entity{entityID, m_Context.get()};
			DrawEntityMode(entity);
		}

		// When mouse is hovering on the "Scene Hierarchy" window and click, clear context
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}

		ImGui::End();

		// If there is context, draw the properties
		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponent(m_SelectionContext);
		}

		ImGui::End();
	}

	// Draw the Entity Nodes with ImGui
	void SceneHierarchyPanel::DrawEntityMode(Entity entity)
	{
		// Getting tag from the component
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		// Check if the entity is selected and set the flag, using bitwise operation as Flags are bitwise
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
			ImGuiTreeNodeFlags_OpenOnArrow;

		// Create a tree node based on the entity and flags
		// TreeNodeEX is a function to create nodes, return true if expaned, false if collpased
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		// if the tree node is opened, handle child nodes (if any)
		if (opened)
		{
			// Example of handling child nodes
			ImGuiDockNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());

			if (opened)
			{
				ImGui::TreePop();
			}

			// Whe you call a TreeNodeEx you need to call the TreePop function to close the node, this works similar
			// to HTML or brackets in a programming language. This is to ensure the stack is proper managed
			// and hierarchy are maintained.
			// Close the Node rendering
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponent(Entity entity)
	{
		// Check if it has tag component
		if (entity.HasComponent<TagComponent>())
		{
			// get the tag string
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			// dynamic allocate char vector for the string
			std::vector<char> buffer(tag.begin(), tag.end());
			buffer.resize(256, '\0'); // resize and set null terminator
			if (ImGui::InputText("Tag", buffer.data(), buffer.size()))
			{
				tag = std::string(buffer.data()); // update component tag
			}
		}

		// check if has transform component
		if (entity.HasComponent<TransformComponent>())
		{
			// Create a Tree node
			/* ImGui needs a unique id for manage the ui elements, the typeid() here returns type information for the type
			 * while hash_Code() generates a unique hash value for that type, this ensures the uniqueness and consistency
			 * with types, casting to void* as ImGui needs a void* as identifier
			 */
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
			                      "Transform"))
			{
				// Get the transform ref
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f); //setup ImGui DragFlot3 input

				// close the node
				ImGui::TreePop();
			}
		}

		/* @brief Adding camera component UI for the property panel */
		// Check if has camera component
		if (entity.HasComponent<CameraComponent>())
		{
			// Draw Camera component on property ui
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				// ImGui Checkbox for set if primary camera
				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeString[] = {"Perspective", "Orthographic"};
				const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];

				// Dropdown menu for selecting the camera type
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					// loop over the projection types
					for (int i = 0; i < 2; i++)
					{
						// Check if it current item is selected
						bool isSelected = currentProjectionTypeString == projectionTypeString[i];

						/* ImGui::Selectable uses "isSelected" to decide how to render it, it returns if the item is
						 * clicked in the process
						 */
						// Check if the item is clicked, if so set it to be current item and update projection
						if (ImGui::Selectable(projectionTypeString[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeString[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						
						// If it is selected, then set it as the default item
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				
				// DragFloat UIs for modify the perspective camera
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFOV))
					{
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFOV));
					}

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
					{
						camera.SetPerspectiveNearClip(perspectiveNear);
					}

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
					{
						camera.SetPerspectiveFarClip(perspectiveFar);
					}
				}

				// DragFloat UIs for modify the orthographic camera
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
					{
						camera.SetOrthographicSize(orthoSize);
					}

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
					{
						camera.SetOrthographicNearClip(orthoNear);
					}

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
					{
						camera.SetOrthographicFarClip(orthoFar);
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}

		// Draw sprite renderer coomponent with a color edit
		if (entity.HasComponent<SpriteRenderComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRenderComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& src = entity.GetComponent<SpriteRenderComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
				ImGui::TreePop();
			}
		}
	}
}
