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
		UniPtr<Camera> cam;
		SharedPtr<Framebuffer> fb;

	};

}