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

			if(opened)
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
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				// Get the transform ref
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f); //setup ImGui DragFlot3 input

				// close the node
				ImGui::TreePop();
			}
		}
	}
}
