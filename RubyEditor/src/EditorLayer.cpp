#include "EditorLayer.h"
#include <ImGui/imgui.h>

namespace Ruby {

	static uint32_t texHeight = 0, texWidth = 0;

	EditorLayer::EditorLayer() : Layer("Editor Layer")
	{

	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::update(double deltaSeconds)
	{
		fb->bind();
		Renderer::API::clear();
		Renderer::updateCamera(*cam);
		Renderer::resetBatch();
		Renderer::drawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {0.8f, 0.2f, 0.3f, 1.0f});
		Renderer::renderBatched();
		fb->unbind();
	}

	void EditorLayer::ImGuiRender()
    {
		static bool dockspaceOpen = true;
		static bool fullscreen = true;
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			windowFlags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, windowFlags);
		ImGui::PopStyleVar();

		if (fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"));
					//OpenProject();

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"));
					//NewScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"));
					//SaveScene();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"));
					//SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"));
					//Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		ImGui::Image((void*)fb->getTextureID(), ImVec2{ (float)texWidth, (float)texHeight });

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::onEvent(Event& e)
	{
		if (e.getType() == EventType::WindowResized)
		{
			WindowResizeEvent& wrEvent = static_cast<WindowResizeEvent&>(e);
			uint32_t width = wrEvent.getWidth(), height = wrEvent.getHeight();
			fb->regenerate(width, height);
			cam->setProjection((float)width / (float)height);
			texHeight = height >> 1;
			texWidth = width >> 1;
		}
	}

	void EditorLayer::onPush()
	{
		auto& wind = App::getInstance().getWindow();
		uint32_t width = wind.getWidth(), height = wind.getHeight();
		cam = createUniPtr<Camera>((float)width / (float)height);
		fb = Framebuffer::create(width, height);
		texHeight = height >> 1;
		texWidth = width >> 1;

	}

	void EditorLayer::onPop()
	{
	}

}