#include "imgui.h"
#include "ruby_pch.h"

#include "ImGuiUtil.h"
#include "Ruby/Main/App.h"
#include "Ruby/Render/Context.h"

// #include <backends/imgui_impl_glfw.h>
// #include <backends/imgui_impl_opengl3.h>
// // FOR WHEN WE ADD VULKAN SUPPORT
// // #include <backends/imgui_impl_vulkan.h>

#pragma warning(push, 0)
#include <backends/imgui_impl_opengl3.cpp>
// FOR WHEN WE ADD VULKAN SUPPORT
// #include <backends/imgui_impl_vulkan.cpp>
#include <backends/imgui_impl_glfw.cpp>
#pragma warning(pop)

namespace Ruby 
{
    namespace ImGuiUtil 
    {
        void init()
        {
            IMGUI_CHECKVERSION();

            // ImGui context and io reference
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            // Allow for keyboard input, gamepad input, docking, and viewports.
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

            ImGui::StyleColorsDark();

            ImGuiStyle& style = ImGui::GetStyle();
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;

            const Window& win = App::getInstance().getWindow();
            switch(Context::getAPI())
            {
            case API::OpenGL:
                ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)win.getWinPointer(), true);
                ImGui_ImplOpenGL3_Init("#version 450 core");
                break;
            case API::Vulkan:
                // ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)win.getWinPointer(), true);
                // ImGui_ImplVulkan_Init();
            default:
                RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
                break;
            }
        }

        void deInit()
        {

            switch(Context::getAPI())
            {
            case API::OpenGL:
                ImGui_ImplOpenGL3_Shutdown();
                break;
            case API::Vulkan:
                // ImGui_ImplVulkan_Shutdown();
            default:
                RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
                break;
            }
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        void begin()
        {
            switch(Context::getAPI())
            {
            case API::OpenGL:
                ImGui_ImplOpenGL3_NewFrame();
                break;
            case API::Vulkan:
                // ImGui_ImplVulkan_NewFrame();
            default:
                RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
                break;
            }
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        void end()
        {
            ImGuiIO& io = ImGui::GetIO();
            const Window& win = App::getInstance().getWindow();
            io.DisplaySize = ImVec2((float)win.getWidth(), (float)win.getHeight());

            ImGui::Render();

            switch(Context::getAPI())
            {
            case API::OpenGL:
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                break;
            case API::Vulkan:
                // ImGui_ImplVulkan_NewFrame();
            default:
                RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
                break;
            }


            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup);
            }
        }

    }

}
