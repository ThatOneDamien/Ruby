#pragma once

#include <Ruby.h>
#include <ImGui/imgui.h>

namespace Ruby 
{
    struct EditorEntity
    {
        Entity EntityID;
        std::string Name = "Unnamed Entity";
        size_t ParentIndex = ULLONG_MAX;
    };

    // Global state of the editor. Yes, globals.
    // So I don't have to deal with adding getters/setters,
    // or making them public and grabbing the app instance evertime.
    // No one should work on this code but me either, so I don't think
    // it is an issue.
    extern bool        g_ShowDebugWindow;
    extern ImVec2      g_PrevViewportSize;
    extern OrthoCamera g_EditorCam;
    extern float       g_CameraZoom;
    extern std::string g_ScenePath;
    extern bool        g_SceneModified;
    extern size_t      g_SelectedEntity;
    extern bool        g_SceneSelected;
    extern SharedPtr<Framebuffer>    g_FBO;
    extern bool                      g_InvalidFB;
    extern SharedPtr<Scene>          g_LoadedScene;
    extern std::vector<EditorEntity> g_EntityList;

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
        virtual void onMouseScroll(double xoff, double yoff) override;
        virtual void onKeyEvent(KeyCode code, int scancode, KeyAction action, int mods) override;
        virtual void onExit() override;

    private:
        void renderInspector();
        void drawStyledControl(const char* label, float* values, 
                               const FloatControl* styles, size_t count, 
                               float resetValue, float dragSpeed,
                               float minValue, float maxValue, float columnWidth);
    private:
    };
}
