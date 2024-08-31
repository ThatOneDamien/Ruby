#include "Ruby/Main/Core.h"
#include "Ruby/Scene/Components.h"
#include "ruby_pch.h"

#include <fstream>
#include <string>
#include <regex>

#include "Scene.h"
#include "Entity.h"
#include "Ruby/Render/Renderer2D.h"

namespace Ruby 
{
    constexpr const char* Scene::DEFAULT_SCENE_NAME;

    Scene::Scene()
        : m_Name(DEFAULT_SCENE_NAME)
    {
    }

    Scene::Scene(const std::string& filepath)
    {
        if (!deserializeFile(filepath))
        {
            RB_ERROR("Unable to deserialize file, creating blank scene.");
            m_Name = DEFAULT_SCENE_NAME;
        }
    }

    Scene::~Scene()
    {
    }

    Entity Scene::createEntity()
    {
        return Entity(this);
    }

    Components::Camera& Scene::getMainCamera() const
    {
        return m_MainCam->getComponent<Components::Camera>();
    }

    std::vector<Entity> Scene::getAllEntities() 
    {
        std::vector<Entity> res;
        m_Registry.each_forward(
                [&](auto e)
                {
                   Entity entity(this, e);
                   res.push_back(entity);
                });
        return res;
    }

    void Scene::setMainCamera(Entity& entityWithMainCam)
    {
        if(!entityWithMainCam.hasComponents<Components::Camera>())
        {
            RB_ERROR("Attempted to set main camera to an entity without camera component.");
            return;
        }
        m_MainCam = &entityWithMainCam;
    }

    void Scene::updateStatic(double deltaMillis)
    {
        Renderer2D::resetBatch();
        {
            auto group = m_Registry.group<Components::Transform>(entt::get<Components::Sprite>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<Components::Transform, Components::Sprite>(entity);
                Renderer2D::drawQuadRotTex(transform.Position, transform.Scale, transform.Rotation, sprite.Tex, sprite.Color);
            }
        }

        Renderer2D::renderBatch();
    }

    void Scene::updateRuntime(double deltaMillis)
    {
        Renderer2D::useCamera(getMainCamera().Cam);
        // performPhysics();
        updateStatic(deltaMillis);
    }


    bool Scene::serialize(const std::string& saveFilePath) const
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

    static size_t extractFloatImpl(const std::string& line, size_t startIndex, float* o_Float)
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

        // Adds check for bad syntax to exit function early.
#define ExtractFloat(line, startIndex, out) if(!(startIndex = extractFloatImpl(line, startIndex, out))) return false;
        

        std::ifstream is(filepath);
        RB_ENSURE_RET(is, false, "Unable to open scene file \'%s\'.", filepath.c_str());

        std::string line;

        std::getline(is, line);
        {
            bool firstLineHash = std::regex_match(line, std::basic_regex("# RUBY SCENE\\s*"));
            RB_ENSURE_RET(firstLineHash, false, "Scene file syntax error. First line should read \'# RUBY SCENE\'.");
        }

        m_MainCam = nullptr;
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
                if(!std::regex_match(line, std::basic_regex("Entity .*")))
                    return false;
                currentEntity = new Entity(this); 
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
                        ExtractFloat(line, index, &transform.Position.x);
                        ExtractFloat(line, index, &transform.Position.y);
                    }
                    else if(std::regex_match(line, std::basic_regex("\t\tRotation:.*")))
                        extractFloatImpl(line, 11, &transform.Rotation);
                    else if(std::regex_match(line, std::basic_regex("\t\tScale:.*")))
                    {
                        size_t index = 8;
                        ExtractFloat(line, index, &transform.Scale.x);
                        ExtractFloat(line, index, &transform.Scale.y);
                    }
                }
                else if(compState == Sprite)
                {
                    Components::Sprite& sprite = currentEntity->getComponent<Components::Sprite>();
                    if(std::regex_match(line, std::basic_regex("\t\tColor:.*")))
                    {
                        size_t index = 8;
                        ExtractFloat(line, index, &sprite.Color.r);
                        ExtractFloat(line, index, &sprite.Color.g);
                        ExtractFloat(line, index, &sprite.Color.b);
                        ExtractFloat(line, index, &sprite.Color.a);
                    }
                }
            }
            else 
                return false;
        }
        return true;
#undef ExtractFloat
    }
}
