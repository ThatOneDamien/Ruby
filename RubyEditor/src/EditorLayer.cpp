#include "EditorLayer.h"
#include <ImGui/imgui.h>

namespace Ruby {

	EditorLayer::EditorLayer() : Layer("Editor Layer")
	{

	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::update(double deltaSeconds)
	{
		glm::vec2 pos = m_Cam.getPosition();
		if (Input::isKeyDown(Key::A))
		{
			pos.x -= 0.05f;
		}
		else if (Input::isKeyDown(Key::D))
		{
			pos.x += 0.05f;
		}

		if (Input::isKeyDown(Key::W))
		{
			pos.y += 0.05f;
		}
		else if (Input::isKeyDown(Key::S))
		{
			pos.y -= 0.05f;
		}

		m_Cam.setPosition(pos);
		m_FBO->bind();
		Renderer::API::clear();
		Renderer::updateCamera(m_Cam);
		Renderer::resetBatch();
		Renderer::drawQuadTexture({0.0f, 0.0f}, m_Tex->getSize()* 0.001f, m_Tex);
		Renderer::renderBatched();
		m_FBO->unbind();
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

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();

		ImGui::Image((void*)m_FBO->getTextureID(), ImVec2{ viewportMaxRegion.x, viewportMaxRegion.x * m_InvAspectRatio}, {0, 1}, {1, 0});

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::onEvent(Event& e)
	{
		switch (e.getType())
		{
			case EventType::WindowResized:
			{
				WindowResizeEvent& wrEvent = static_cast<WindowResizeEvent&>(e);
				uint32_t width = wrEvent.getWidth(), height = wrEvent.getHeight();
				m_FBO->regenerate(width, height);
				m_InvAspectRatio = (float)height / (float)width;
				m_Cam.setProjection((float)width / (float)height);
				break;
			}
			case EventType::MouseScroll:
			{
				MouseScrollEvent& msEvent = static_cast<MouseScrollEvent&>(e);
				break;
			}
		}
	}

	void EditorLayer::onPush()
	{
		m_Tex = Texture::createTexture("res/images/poop.jpg");

		auto& wind = App::getInstance().getWindow();
		uint32_t width = wind.getWidth(), height = wind.getHeight();
		m_FBO = Framebuffer::create(width, height);
		m_InvAspectRatio = (float)height / (float)width;


		m_Cam.setProjection((float)width / (float)height);
	}

	void EditorLayer::onPop()
	{
	}

}