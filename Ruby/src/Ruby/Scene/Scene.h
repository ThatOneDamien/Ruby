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


        Entity createEntity();

        Components::Camera& getMainCamera() const;
        std::vector<Entity> getAllEntities();
        inline const std::string& getName() const { return m_Name; }

        void setMainCamera(Entity& entityWithMainCam);

        // Does not perform physics updates, and leaves the
        // camera as the one attached to the renderer.
        void updateStatic(double deltaMillis);
        // Performs physics updates, binds the main camera,
        // and renders the scene.
        void updateRuntime(double deltaMillis);

        


        bool serialize(const std::string& saveLocation) const;

        // Deserializes the specified file from disk and places its contents into
        // the calling Scene object. If the Scene object had previous data, it is
        // cleared and replaced with the new Scene data from the file. Returns true
        // if successful, false otherwise.
        bool deserializeFile(const std::string& filepath);

        template<typename func>
        inline void forEachForward(func f) 
        {
            static_assert(std::is_invocable_v<func, entt::registry::entity_type>);
            m_Registry.each_forward(f);
        }
        
        template<typename func>
        inline void forEachBackward(func f) 
        {
            static_assert(std::is_invocable_v<func, entt::registry::entity_type>);
            m_Registry.each_backward(f);
        }

    private:

    private:
        static constexpr const char* DEFAULT_SCENE_NAME = "Untitled Scene";
        friend class Entity;
        friend class SceneRuntime;

        std::string m_Name;
        entt::registry m_Registry;
        Entity* m_MainCam{nullptr};
    };

}
