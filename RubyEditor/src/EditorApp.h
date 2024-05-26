#pragma once


namespace Ruby 
{
	struct EditorEntity
	{
		Entity entity;
		std::string name = "Unnamed Entity";
		size_t parentIndex = ULLONG_MAX;
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
	private:
		Camera m_Cam{ 1.0f,1.0f };
		SharedPtr<Texture> m_Tex;
		SharedPtr<Framebuffer> m_FBO;
		SharedPtr<Scene> m_LoadedScene;
		std::vector<EditorEntity> m_EntityList;
		size_t m_SelectedEntity = ULLONG_MAX;
		bool m_SceneSelected = false;
		float m_InvAspectRatio = 1.0f;
		float m_Scale = 1.0f;
	};
}