#include "ruby_pch.h"

#include <fstream>

#include "Scene.h"
#include "Entity.h"
#include "Ruby/Render/Renderer.h"

namespace Ruby 
{
	Scene::Scene(const std::string& name)
		: m_Name(name)
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::updateScene(double deltaMillis)
	{
		{
			auto group = m_Registry.group<Components::Transform>(entt::get<Components::Sprite>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<Components::Transform, Components::Sprite>(entity);
				Renderer::drawQuadRotTex(transform.Position, transform.Scale, transform.Rotation, sprite.Tex, sprite.Color);
			}
		}

	}

	Entity Scene::createEntity()
	{
		return Entity(this);
	}

	bool Scene::serializeScene()
	{
		return serializeScene(m_Name + ".rusc");
	}

	bool Scene::serializeScene(const std::string& saveFilePath)
	{
		std::ofstream out(saveFilePath);
		if (!out)
		{
			RB_ERROR("Unable to create file for scene serialization.");
			return false;
		}
		m_Registry.each_forward([&](auto e) {
			out << "Entity " << entt::to_integral(e) << ":\n";
			if (m_Registry.all_of<Components::Transform>(e))
			{
				const Components::Transform& transform = m_Registry.get<Components::Transform>(e);
				out << "\tTransform:\n";
				out << "\t\tPosition: " << transform.Position.x << ',' << transform.Position.y << '\n';

				out << "\t\tRot: " << transform.Rotation << '\n';
				out << "\t\tScale: " << transform.Scale.x << ' ' << transform.Scale.y << '\n';
			}
		});
		out.flush();
		out.close();
		return true;
	}


}
