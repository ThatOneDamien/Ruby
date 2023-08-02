#pragma once

#include <Ruby.h>

namespace Ruby {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void update(double deltaSeconds) override;
		virtual void ImGuiRender() override;
		virtual void onEvent(Event& e) override;
		virtual void onPush() override;
		virtual void onPop() override;

	private:
		Camera m_Cam{1.0f,1.0f};
		SharedPtr<Texture> m_Tex;
		SharedPtr<Framebuffer> m_FBO;
		float m_InvAspectRatio = 1.0f;
	};

}