#pragma once

#include "Scene.h"

namespace Ruby {

	class Entity 
	{
		static constexpr uint32_t nullID = (uint32_t)entt::tombstone;
	public:
		Entity(Scene* scene);
		~Entity() = default;

		inline uint32_t getID() const { return (uint32_t)m_EntityID; }


		template<typename... comp_t>
		bool hasComponents()
		{
			return m_Scene->m_Registry.all_of<comp_t...>(m_EntityID);
		}

		template<typename comp_t, typename... Args>
		comp_t& addComponent(Args&&... args)
		{
			RB_ASSERT(!hasComponents<comp_t>(), "Attempted to add duplicate component to entity.");
			return m_Scene->m_Registry.emplace<comp_t>(m_EntityID, std::forward<Args>(args)...);
		}

		template<typename comp_t>
		comp_t& getComponent()
		{
			RB_ASSERT(hasComponents<comp_t>(), "Attempted to get non-existent component.");
			return m_Scene->m_Registry.get<comp_t>(m_EntityID);
		}

	private:
		friend class Scene;

		entt::entity m_EntityID;
		Scene* m_Scene;

	};

}