#include "ruby_pch.h"

#include <fstream>
#include <string>
#include <regex>

#include "Scene.h"
#include "Entity.h"
#include "Ruby/Render/Renderer.h"

namespace Ruby 
{
    constexpr const char* Scene::DEFAULT_SCENE_NAME;

    Scene::Scene()
        : m_Filepath("untitled_scene.rusc"), m_Name(DEFAULT_SCENE_NAME)
    {
    }

    Scene::Scene(const std::string& filepath)
    {
        if (deserializeFile(filepath))
        {

        }
        else
        {
            RB_ERROR("Unable to deserialize file, creating blank scene.");
            m_Filepath = "untitled_scene.rusc";
            m_Name = DEFAULT_SCENE_NAME;
        }
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

    bool Scene::serialize()
    {
        return serialize(m_Filepath);
    }

    bool Scene::serialize(const std::string& saveFilePath)
    {
        std::ofstream out(saveFilePath);
        if (!out)
        {
            RB_ERROR("Unable to create file for scene serialization.");
            return false;
        }

        out << "# RUBY SCENE\n# NAME: " << m_Name << '\n';

        m_Registry.each_forward(
        [&](auto e) 
        {
            if (!m_Registry.any_of<Components::Transform, Components::Sprite>(e))
                return;
            out << "Entity " << entt::to_integral(e) << ":\n";
            if (m_Registry.all_of<Components::Transform>(e))
            {
                const Components::Transform& transform = m_Registry.get<Components::Transform>(e);
                out << "\tTransform:\n";
                out << "\t\tPosition: " << transform.Position.x << ',' << transform.Position.y << '\n';
                out << "\t\tRot: " << transform.Rotation << '\n';
                out << "\t\tScale: " << transform.Scale.x << ',' << transform.Scale.y << '\n';
            }
            if (m_Registry.all_of<Components::Sprite>(e))
            {
                const Components::Sprite& sprite = m_Registry.get<Components::Sprite>(e);
                out << "\tSprite:\n";
                out << "\t\tColor: " << sprite.Color.r << ',' << sprite.Color.g << ',' << sprite.Color.b << ',' << sprite.Color.a << '\n';
                if (sprite.Tex)
                    out << "\t\tTexture: " << sprite.Tex->getFilepath() << '\n';
            }
        });
        out.flush();
        out.close();
        return true;
    }

    bool Scene::deserializeFile(const std::string& filepath)
    {
        std::ifstream is(filepath);
        if (!is)
        {
            RB_ERROR("Unable to open scene file \'%s\'.", filepath.c_str());
            return false;
        }

        std::string line;

        std::getline(is, line);
        if (!std::regex_match(line, std::basic_regex("# RUBY SCENE\\s*")))
        {
            RB_ERROR("Scene file syntax error. First line should read \'# RUBY SCENE\'.");
            return false;
        }

        m_Filepath = filepath;
        m_MainCamera = nullptr;
        m_Registry.clear();

        std::getline(is, line);
        m_Name = std::regex_match(line, std::basic_regex("# NAME .*")) ? line.substr(7) : DEFAULT_SCENE_NAME; // '# NAME ' is 7 characters long
        while (std::getline(is, line)) 
        {

        }
        return true;
    }
}
