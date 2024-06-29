#include "ruby_pch.h"

#include "Entity.h"

namespace Ruby 
{
    Entity::Entity(Scene* scene)
        : m_Scene(scene)
    {
        // TODO: Make sure this static assert never fails, and if it does, find a way to make it not.
        static_assert(std::is_same_v<ENTT_ID_TYPE, uint32_t>, "Entt type not equivalent to 32 bit integer.");

        m_EntityID = m_Scene->m_Registry.create();
    }
}