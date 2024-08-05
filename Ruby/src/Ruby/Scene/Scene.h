#pragma once

#include <entt/entt.hpp>
#include "Components.h"



namespace Ruby 
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        Scene(const std::string& filepath);
        ~Scene();

        // Call this function within Renderer::resetBatch() and Renderer::renderBatched()
        void updateScene(double deltaMillis);

        Entity createEntity();

        bool serialize();
        bool serialize(const std::string& saveLocation);

        // Deserializes the specified file from disk and places its contents into
        // the calling Scene object. If the Scene object had previous data, it is
        // cleared and replaced with the new Scene data from the file. Returns true
        // if successful, false otherwise.
        bool deserializeFile(const std::string& filepath);

    private:
        static constexpr const char* DEFAULT_SCENE_NAME = "Untitled Scene";
        static constexpr const char* DEFAULT_SCENE_FILEPATH = "untitled_scene.rusc";
        friend class Entity;

        std::string m_Filepath;
        std::string m_Name;
        entt::registry m_Registry;
        Entity* m_MainCamera{nullptr};
    };

}
