#pragma once

namespace Ruby {

	class EditorApp : public App
	{
	public:
		EditorApp(int argc, char** argv, const std::string& mainDirectory, const char* name, int width, int height)
			: App(argc, argv, mainDirectory, name, width, height)
		{
		}

		virtual void onEvent(Event& e) override;
		virtual void update(double deltaMillis) override;
		virtual void ImGuiRender(double deltaMillis) override;
		virtual void onStart() override;
		virtual void onExit() override;

	private:
		Camera m_Cam{ 1.0f,1.0f };
		SharedPtr<Texture> m_Tex;
		SharedPtr<Framebuffer> m_FBO;
		SharedPtr<Scene> m_Scene;
		float m_InvAspectRatio = 1.0f;
		float m_Scale = 1.0f;
	};
}