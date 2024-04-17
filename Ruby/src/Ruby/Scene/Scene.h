#pragma once

#include <entt/entt.hpp>
#include "Components.h"

namespace Ruby {

	class Entity;

	class Scene
	{
	public:
		Scene(const std::string& name);
		//TODO: Make constructor for scene with filename
		~Scene();

		void updateScene(double deltaMillis);

		Entity createEntity();

	private:
		friend class Entity;


		std::string m_Name;
		entt::registry m_Registry;
	};

}