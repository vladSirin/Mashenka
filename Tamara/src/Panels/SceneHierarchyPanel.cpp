#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include "Mashenka/Scene/Component.h"

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
}
