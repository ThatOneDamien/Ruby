#include "Ruby/Main/Core.h"
#include "Ruby/Scene/Components.h"
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
    constexpr const char* Scene::DEFAULT_SCENE_FILEPATH;

    Scene::Scene()
        : m_Filepath(DEFAULT_SCENE_FILEPATH), m_Name(DEFAULT_SCENE_NAME)
    {
    }

    Scene::Scene(const std::string& filepath)
    {
        m_Filepath = filepath;
        if (!deserializeFile(filepath))
        {
            RB_ERROR("Unable to deserialize file, creating blank scene.");
            m_Filepath = DEFAULT_SCENE_FILEPATH;
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
            // TODO: Replace the entity number with a UUID
            out << "Entity " << entt::to_integral(e) << ":\n";
            if (m_Registry.all_of<Components::Transform>(e))
            {
                const Components::Transform& transform = m_Registry.get<Components::Transform>(e);
                out << "\tTransform:\n";
                out << "\t\tPosition: " << transform.Position.x << ',' << transform.Position.y << '\n';
                out << "\t\tRotation: " << transform.Rotation << '\n';
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

    static size_t extractFloat(const std::string& line, size_t startIndex, float* o_Float)
    {
        startIndex = line.find_first_of("0123456789.", startIndex);
        if(startIndex == std::string::npos)
            return 0;
        size_t endIndex = line.find_first_not_of("0123456789.", startIndex);
        if(endIndex == std::string::npos)
            endIndex = line.size();
        std::string str = line.substr(startIndex, endIndex - startIndex);
        *o_Float = std::stof(str);
        return endIndex + 1;
    }

    bool Scene::deserializeFile(const std::string& filepath)
    {
        std::ifstream is(filepath);
        RB_ASSERT_RET(is, false, "Unable to open scene file \'%s\'.", filepath.c_str());

        std::string line;

        std::getline(is, line);
        {
            bool firstLineHash = std::regex_match(line, std::basic_regex("# RUBY SCENE\\s*"));
            RB_ASSERT_RET(firstLineHash, false, "Scene file syntax error. First line should read \'# RUBY SCENE\'.");
        }

        m_Filepath = filepath;
        m_MainCamera = nullptr;
        m_Registry.clear();

        if(std::getline(is, line))
            m_Name = std::regex_match(line, std::basic_regex("# NAME: .*")) ? line.substr(8) : DEFAULT_SCENE_NAME;

        Entity* currentEntity = nullptr;
        enum ComponentState
        {
            None,
            Transform,
            Sprite,
            Camera
        };
        
        ComponentState compState = None;
        size_t lineNum = 2;
        // TODO: Add debug messages
        while (std::getline(is, line)) 
        {
            ++lineNum;
            if(line.empty())
                continue;
            if(line[0] != '\t')
            {
                if(std::regex_match(line, std::basic_regex("Entity .*")))
                    currentEntity = new Entity(this); 
                else
                    return false;
            }
            else if(!currentEntity)
                return false;
            else if(line[1] != '\t')
            {
                if(std::regex_match(line, std::basic_regex("\tTransform:\\s*")))
                {
                    currentEntity->addComponent<Components::Transform>();
                    compState = Transform; 
                }
                else if(std::regex_match(line, std::basic_regex("\tSprite:\\s*")))
                {
                    currentEntity->addComponent<Components::Sprite>();
                    compState = Sprite;
                }
                else if(std::regex_match(line, std::basic_regex("\tCamera:\\s*")))
                {
                    currentEntity->addComponent<Components::Camera>();
                    compState = Camera; 
                }
                else
                    return false;
            }
            else if(compState == None)
                return false;
            else if(line[2] != '\t')
            {
                if(compState == Transform)
                {
                    Components::Transform& transform = currentEntity->getComponent<Components::Transform>();
                    if(std::regex_match(line, std::basic_regex("\t\tPosition:.*")))
                    {
                        size_t index = 11;
                        index = extractFloat(line, index, &transform.Position.x);
                        if(!index)
                            RB_ERROR("Incorrect syntax in file \'%s\'. Please check line %lu.", filepath.c_str(), lineNum);
                        index = extractFloat(line, index, &transform.Position.y);
                        if(!index)
                            RB_ERROR("Incorrect syntax in file \'%s\'. Please check line %lu.", filepath.c_str(), lineNum);
                    }
                    else if(std::regex_match(line, std::basic_regex("\t\tRotation:.*")))
                        extractFloat(line, 11, &transform.Rotation);
                    else if(std::regex_match(line, std::basic_regex("\t\tScale:.*")))
                    {
                        size_t index = 8;
                        index = extractFloat(line, index, &transform.Scale.x);
                        if(!index)
                            RB_ERROR("Incorrect syntax in file \'%s\'. Please check line %lu.", filepath.c_str(), lineNum);
                        index = extractFloat(line, index, &transform.Scale.y);
                        if(!index)
                            RB_ERROR("Incorrect syntax in file \'%s\'. Please check line %lu.", filepath.c_str(), lineNum);
                    }
                }
                else if(compState == Sprite)
                {
                    Components::Sprite& sprite = currentEntity->getComponent<Components::Sprite>();
                    if(std::regex_match(line, std::basic_regex("\t\tColor:.*")))
                    {
                        size_t index = 8;
                        index = extractFloat(line, index, &sprite.Color.r);
                        if(!index)
                            RB_ERROR("Incorrect syntax in file \'%s\'. Please check line %lu.", filepath.c_str(), lineNum);
                        index = extractFloat(line, index, &sprite.Color.g);
                        if(!index)
                            RB_ERROR("Incorrect syntax in file \'%s\'. Please check line %lu.", filepath.c_str(), lineNum);
                        index = extractFloat(line, index, &sprite.Color.b);
                        if(!index)
                            RB_ERROR("Incorrect syntax in file \'%s\'. Please check line %lu.", filepath.c_str(), lineNum);
                        index = extractFloat(line, index, &sprite.Color.a);
                        if(!index)
                            RB_ERROR("Incorrect syntax in file \'%s\'. Please check line %lu.", filepath.c_str(), lineNum);
                    }
                }
            }
            else 
                return false;
        }
        return true;
    }
}
