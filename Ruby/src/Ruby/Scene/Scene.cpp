#include "ruby_pch.h"

#include "Scene.h"
#include "Entity.h"
#include "Ruby/Render/Renderer.h"

namespace Ruby {

	Scene::Scene(const std::string& name)
		: m_Name(name)
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::updateScene(double deltaMillis)
	{
		auto group = m_Registry.group<Components::Transform>(entt::get<Components::Sprite>);
		for (auto entity : group) 
		{
			auto&[transform, sprite] = group.get<Components::Transform, Components::Sprite>(entity);
			Renderer::drawQuadRotTex(transform.Position, transform.Scale, transform.Rotation, sprite.Tex, sprite.Color);
		}
	}

	Entity Scene::createEntity()
	{
		return Entity(this);
	}


}
