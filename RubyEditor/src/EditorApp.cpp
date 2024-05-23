#include <Ruby.h>
#include <Ruby/Main/MainFunctionEntry.h>
#include "EditorApp.h"
#include <ImGui/imgui.h>

namespace Ruby {

	void EditorApp::onStart()
	{
		m_Tex = Texture::createTexture("res/images/poop.jpg");
		m_LoadedScene = createShared<Scene>("Unnamed");

		auto& wind = App::getInstance().getWindow();
		uint32_t width = wind.getWidth(), height = wind.getHeight();
		m_FBO = Framebuffer::create(width, height);
		m_InvAspectRatio = (float)height / (float)width;


		m_Cam.setProjection((float)width / (float)height);
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("res/fonts/Nunito-Regular.ttf", 20);
		Renderer::useCamera(m_Cam);
		m_EntityList.push_back({ m_LoadedScene->createEntity(), "Bruh1" });
		m_EntityList[0].entity.addComponent<Components::Transform>();
		m_EntityList.push_back({ m_LoadedScene->createEntity(), "Bruh2" });
		m_EntityList.push_back({ m_LoadedScene->createEntity(), "Bruh3" });
		m_EntityList.push_back({ m_LoadedScene->createEntity(), "Bruh4" });
		
	}

	void EditorApp::update(double deltaMillis)
	{
		if (m_SceneSelected)
		{
			glm::vec2 pos = m_Cam.getPosition();
			pos.x += m_Scale * 0.02f * (Input::isKeyDown(Key::D) - Input::isKeyDown(Key::A));
			pos.y += m_Scale * 0.02f * (Input::isKeyDown(Key::W) - Input::isKeyDown(Key::S));
			m_Cam.setPosition(pos);
		}
		Renderer::API::clear();
		m_FBO->bind();
		Renderer::API::clear();
		Renderer::resetBatch();
		Renderer::drawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
		Renderer::renderBatch();
		m_FBO->unbind();
	}

	void EditorApp::ImGuiRender(double deltaMillis)
	{
		//ImGui::ShowDemoWindow();
		static bool dockspaceOpen = true;
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			windowFlags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Ruby2D Dockspace", &dockspaceOpen, windowFlags);
		ImGui::PopStyleVar(3);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {144.0f, 144.0f});
			ImGuiID dockspace_id = ImGui::GetID("Ruby2D Dockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
			ImGui::PopStyleVar();
		}


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{


				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// For now not showing the full tree
		// i.e. the parent entities have no effect
		// all entities are at the same level.
		if (ImGui::Begin("Entity Tree")) 
		{
			for (size_t i = 0; i < m_EntityList.size(); ++i)
			{
				if (ImGui::Selectable(m_EntityList[i].name.c_str(), m_SelectedEntity == i))
					m_SelectedEntity = i;
				// FOR WHEN I IMPLEMENT THE FULL ENTITY TREE
				/*if (ImGui::TreeNode(e.name.c_str()))
				{
					ImGui::TreePop();
				}*/
			}
			if (ImGui::Button("Add Entity"))
			{
				std::string s("Bruh");
				s += std::to_string(m_EntityList.size() + 1);
				m_EntityList.push_back({ m_LoadedScene->createEntity(), s.c_str() });
			}
		}
		ImGui::End();

		if (ImGui::Begin("Inspector"))
		{
			renderInspector();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		if (ImGui::Begin("Scene"))
		{
			m_SceneSelected = ImGui::IsWindowFocused();
			ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();

			ImGui::Image((ImTextureID)(uint64_t)m_FBO->getTextureID(),
				ImVec2{ viewportMaxRegion.x, viewportMaxRegion.x * m_InvAspectRatio },
				{ 0, 1 }, { 1, 0 });
		}
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorApp::onEvent(Event& e)
	{
		switch (e.getType())
		{
		case EventType::WindowResized:
		{
			WindowResizeEvent& wrEvent = static_cast<WindowResizeEvent&>(e);
			uint32_t width = wrEvent.getWidth(), height = wrEvent.getHeight();
			if (width && height)
			{
				m_FBO->regenerate(width, height);
				m_InvAspectRatio = (float)height / (float)width;
				m_Cam.setProjection((float)width / (float)height);
			}
			break;
		}
		case EventType::MouseScroll:
		{
			if (m_SceneSelected)
			{
				MouseScrollEvent& msEvent = static_cast<MouseScrollEvent&>(e);
				m_Scale -= 0.05f * m_Scale * msEvent.getYOffset();
				m_Cam.setProjection(1 / m_InvAspectRatio, m_Scale);
			}
			break;
		}
		}
	}

	void EditorApp::onExit()
	{

	}

	void EditorApp::renderInspector()
	{
		if (m_EntityList.empty() || m_SelectedEntity >= m_EntityList.size())
			return;

		Entity& e = m_EntityList[m_SelectedEntity].entity;
		float width = ImGui::CalcItemWidth();
		if (e.hasComponents<Components::Transform>())
		{
			Components::Transform& transform = e.getComponent<Components::Transform>();
			if (ImGui::TreeNode("Transform"))
			{
				ImGui::BeginTable("a", 4);
				ImGui::EndTable();
				ImGui::Text("Position");
				ImGui::TextColored({0.0f, 1.0f, 1.0f, 1.0f}, "X");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(width * 0.5f);
				ImGui::InputFloat("##X", &transform.Position.x);
				ImGui::SameLine();
				ImGui::Text("Y");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(width * 0.5f);
				ImGui::InputFloat("##Y", &transform.Position.y);
				ImGui::Text("Rotation");
				ImGui::Text("X");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(width);
				ImGui::InputFloat("##Rotation", &transform.Position.x);
				ImGui::TreePop();
			}
		}

	}
	
}

Ruby::App* createApp(int argc, char** argv)
{
	Ruby::AppSpec spec;
	spec.MainDirectory = "../../../../RubyEditor";
	spec.WindowSpec.Name = "Ruby Editor";
	spec.WindowSpec.Width = 1280;
	spec.WindowSpec.Height = 720;
	spec.WindowSpec.MinWidth = 640;
	spec.WindowSpec.MinHeight = 360;
	spec.WindowSpec.VSync = true;
	spec.WindowSpec.HasTitleBar = true;
	return new Ruby::EditorApp(argc, argv, spec);
}