#include "ruby_pch.h"

#include "Ruby/Main/App.h"

#include "Ruby/GUI/ImGuiLayer.h"

#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

namespace Ruby {

	namespace ImGuiLayer {

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
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			const Window& win = App::getInstance().getWindow();
			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)win.getWinPointer(), true);
			ImGui_ImplOpenGL3_Init("#version 410 core");
		}
		void deInit()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
		void begin()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		void end()
		{
			ImGuiIO& io = ImGui::GetIO();
			const Window& win = App::getInstance().getWindow();
			io.DisplaySize = ImVec2((float)win.getWidth(), (float)win.getHeight());

			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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