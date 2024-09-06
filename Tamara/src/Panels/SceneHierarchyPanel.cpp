#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include "Mashenka/Scene/Component.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_internal.h>

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

		std::vector<Entity> entitiesToDelete;

		// loop over all entities, create entity based on context and Draw them
		for (auto entityID : storage)
		{
			Entity entity{entityID, m_Context.get()};
			if (entity)
			{
				DrawEntityNodeAndMarkDelete(entity, entitiesToDelete);
			}
		}

		for (auto entity : entitiesToDelete)
		{
			m_Context->DestroyEntity(entity);

			// clear selection if the deleted entity was selected
			if (m_SelectionContext == entity)
			{
				m_SelectionContext = {};
			}
		}

		// When mouse is hovering on the "Scene Hierarchy" window and click, clear context
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}

		// Right Click on empty space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		// If there is context, draw the properties
		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponent(m_SelectionContext);

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Add Component");
			}

			if (ImGui::BeginPopup("Add Component"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectionContext.AddComponent<SpriteRenderComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	// Draw the Entity Nodes with ImGui
	void SceneHierarchyPanel::DrawEntityNodeAndMarkDelete(Entity entity, std::vector<Entity>& entitiesToDelete)
	{
		// Getting tag from the component
		if (entity.HasComponent<TagComponent>())
		{
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

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete Entity"))
				{
					entitiesToDelete.push_back(entity);
				}

				ImGui::EndPopup();
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
		else
		{
			//ImGui::Text("Unnamed Entity");
		}
		
	}


	// Draws a control interface for modifying a glm::vec3 with ImGui.
	// This function displays a label and three controls (X, Y, Z) for a glm::vec3,
	// allowing the user to adjust the vector's components via drag controls and reset buttons.
	//
	// Parameters:
	// - label: The label to display next to the control. This is used as a unique identifier within ImGui.
	// - values: A reference to the glm::vec3 variable that will be modified by the control.
	// - resetValue: The value to reset each component (X, Y, Z) to when the corresponding reset button is clicked. Defaults to 0.0f.
	// - columnWidth: The width of the first column where the label is displayed. Defaults to 100.0f.
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f,
	                            float columnWidth = 100.0f)
	{
		// Push a unique ID to prevent ImGui ID collisions when using multiple controls with the same label
		ImGui::PushID(label.c_str());

		// Set up a two-column layout, with the first column width defined by columnWidth
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str()); // Display the label in the first column
		ImGui::NextColumn();

		// Setup the width for the next three items (X, Y, Z controls) and style for no item spacing
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0}); // Remove spacing between controls

		// Calculate the size of the buttons based on the current line height
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight}; // Slightly larger than the line height for better UX

		// Push styles for the 'X' button (red color)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		if (ImGui::Button("X", buttonSize)) // Create a button labeled 'X'
		{
			values.x = resetValue; // Reset X value to the resetValue if the button is clicked
		}
		ImGui::PopStyleColor(3); // Pop the last 3 color styles applied

		ImGui::SameLine(); // Keep the next item on the same line as the button
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		// Create a draggable float for X with fine-tuned control
		ImGui::PopItemWidth(); // Pop width for this item
		ImGui::SameLine();

		// Push styles for the 'Y' button (green color)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
		if (ImGui::Button("Y", buttonSize)) // Create a button labeled 'Y'
		{
			values.y = resetValue; // Reset Y value to the resetValue if the button is clicked
		}
		ImGui::PopStyleColor(3); // Pop the last 3 color styles applied

		ImGui::SameLine(); // Keep the next item on the same line as the button
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		// Create a draggable float for Y with fine-tuned control
		ImGui::PopItemWidth(); // Pop width for this item
		ImGui::SameLine();

		// Push styles for the 'Z' button (blue color)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
		if (ImGui::Button("Z", buttonSize)) // Create a button labeled 'Z'
		{
			values.z = resetValue; // Reset Z value to the resetValue if the button is clicked
		}
		ImGui::PopStyleColor(3); // Pop the last 3 color styles applied

		ImGui::SameLine(); // Keep the next item on the same line as the button
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		// Create a draggable float for Z with fine-tuned control
		ImGui::PopItemWidth(); // Pop width for this item

		ImGui::PopStyleVar(); // Pop the item spacing style
		ImGui::Columns(1); // Restore to a single-column layout
		ImGui::PopID(); // Pop the ID to restore the previous state and avoid ID collisions
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

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		// check if has transform component
		if (entity.HasComponent<TransformComponent>())
		{
			// Create a Tree node
			/* ImGui needs a unique id for manage the ui elements, the typeid() here returns type information for the type
			 * while hash_Code() generates a unique hash value for that type, this ensures the uniqueness and consistency
			 * with types, casting to void* as ImGui needs a void* as identifier
			 */
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
			if (open)
			{
				// Get and set the translation, rotation and scale values with UI
				auto& tc = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", tc.Translation);

				glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);

				DrawVec3Control("Scale", tc.Scale, 1.0f);

				// close the node
				ImGui::TreePop();
			}
		}

		/* @brief Adding camera component UI for the property panel */
		// Check if has camera component
		if (entity.HasComponent<CameraComponent>())
		{
			// Draw Camera component on property ui
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera"))
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
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRenderComponent).hash_code(), treeNodeFlags,
			                              "Sprite Renderer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("+", ImVec2{20, 20}))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}

			if (open)
			{
				auto& src = entity.GetComponent<SpriteRenderComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<SpriteRenderComponent>();
			}
		}
	}
}
