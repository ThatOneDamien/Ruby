#pragma once

#include <entt/entt.hpp>
#include "Components.h"

namespace Ruby 
{
	class Entity;

	class Scene
	{
	public:
		Scene(const std::string& name);
		//TODO: Make constructor for scene with filename
		//Scene(const std::string& filepath);
		~Scene();

		// Call this function within Renderer::resetBatch() and Renderer::renderBatched()
		void updateScene(double deltaMillis);

		Entity createEntity();
		bool serializeScene();
		bool serializeScene(const std::string& saveLocation);

	private:
		friend class Entity;

		std::string m_Name;
		entt::registry m_Registry;
		Entity* m_MainCamera{nullptr};
	};

}