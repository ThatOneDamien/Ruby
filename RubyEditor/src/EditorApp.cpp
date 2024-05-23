#include <Ruby.h>
#include <Ruby/Main/MainFunctionEntry.h>
#include "EditorApp.h"
#include <ImGui/imgui.h>

namespace Ruby {

	void EditorApp::onEvent(Event& e)
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
			m_Scale -= 0.05f * m_Scale * msEvent.getYOffset();
			m_Cam.setProjection(1 / m_InvAspectRatio, m_Scale);
			break;
		}
		}
	}
	void EditorApp::update(double deltaMillis)
	{
		glm::vec2 pos = m_Cam.getPosition();
		pos.x += 0.05f * (Input::isKeyDown(Key::D) - Input::isKeyDown(Key::A));
		pos.y += 0.05f * (Input::isKeyDown(Key::W) - Input::isKeyDown(Key::S));

		m_Cam.setPosition(pos);
		m_FBO->bind();
		Renderer::API::clear();
		Renderer::updateCamera(m_Cam);
		Renderer::resetBatch();
		Renderer::drawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		Renderer::renderBatch();
		m_FBO->unbind();
	}
	void EditorApp::ImGuiRender(double deltaMillis)
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


				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Inspector");
		if (ImGui::TreeNode("Balls"))
		{
			ImGui::TreePop();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Scene");
		ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();

		ImGui::Image((ImTextureID)(uint64_t)m_FBO->getTextureID(), ImVec2{ viewportMaxRegion.x, viewportMaxRegion.x * m_InvAspectRatio }, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}
	void EditorApp::onStart()
	{
		m_Tex = Texture::createTexture("res/images/poop.jpg");
		m_Scene = createShared<Scene>("Unnamed");

		auto& wind = App::getInstance().getWindow();
		uint32_t width = wind.getWidth(), height = wind.getHeight();
		m_FBO = Framebuffer::create(width, height);
		m_InvAspectRatio = (float)height / (float)width;


		m_Cam.setProjection((float)width / (float)height);
	}
	void EditorApp::onExit()
	{

	}

	
}

Ruby::App* createApp(int argc, char** argv)
{
	return new Ruby::EditorApp(argc, argv, "../../../../RubyEditor", "Ruby Editor", 1280, 720);
}