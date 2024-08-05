#pragma once

#include <Ruby.h>

namespace Ruby 
{
    struct EditorEntity
    {
        Entity EntityID;
        std::string Name = "Unnamed Entity";
        size_t ParentIndex = ULLONG_MAX;
    };

    struct FloatControl
    {
        char Label[4];
        glm::vec4 Color;
    };

    class EditorApp : public App
    {
    public:
        EditorApp(int argc, char** argv, const AppSpec& spec)
            : App(argc, argv, spec)
        {
        }

        virtual void onStart() override;
        virtual void update(double deltaMillis) override;
        virtual void ImGuiRender(double deltaMillis) override;
        virtual void onEvent(Event & e) override;
        virtual void onExit() override;

    private:
        void renderInspector();
        void drawStyledControl(const char* label, float* values, 
                               const FloatControl* styles, size_t count, 
                               float resetValue, float dragSpeed,
                               float minValue, float maxValue, float columnWidth);
    private:
        Camera m_Cam{ 1.0f,1.0f };
        SharedPtr<Framebuffer> m_FBO;
        SharedPtr<Scene> m_LoadedScene;
        std::vector<EditorEntity> m_EntityList;
        size_t m_SelectedEntity = ULLONG_MAX;
        bool m_SceneSelected = false;
        float m_InvAspectRatio = 1.0f;
        float m_Scale = 1.0f;
    };
}
