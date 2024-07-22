#pragma once
#include "Mashenka/Core/Base.h"
#include "Mashenka/Core/Log.h"
#include "Mashenka/Scene/Scene.h"
#include "Mashenka/Scene/Entity.h"

namespace Mashenka
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetConText(const Ref<Scene>& context);
		void OnImGuiRender();

	private:
		void DrawEntityMode(Entity entity);
		void DrawComponent(Entity entity);

		Ref<Scene> m_Context; //it needs the scene ptr as context
		Entity m_SelectionContext; //the entity that is selected
	
	};
}

