#include <Ruby.h>
#include <Ruby/Main/MainFunctionEntry.h>
#include <string>

class Sandbox : public Ruby::App
{
	template<typename T>
	using SP = Ruby::SharedPtr<T>;
public:
	Sandbox(int argc, char** argv, const Ruby::AppSpec& spec)
		: Ruby::App(argc, argv, spec)
	{
	}

	virtual ~Sandbox()
	{
	}

	virtual void onStart() override 
	{
		Ruby::Renderer::useCamera(cam);
		aspectRatio = Ruby::App::getInstance().getWindow().getAspectRatio();
		Ruby::Renderer::API::setClearColor({0.2f, 0.2f, 0.2f});
	}

	virtual void update(double deltaMillis) override 
	{
		updateInputs();
		Ruby::Renderer::API::clear();
		Ruby::Renderer::resetBatch();
		Ruby::Renderer::drawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f});
		Ruby::Renderer::renderBatch();
		
	}

	virtual void ImGuiRender(double deltaMillis) override
	{

	}

	virtual void onEvent(Ruby::Event& e) override
	{
		if (e.getType() == Ruby::EventType::MouseScroll)
		{
			Ruby::MouseScrollEvent& l = (Ruby::MouseScrollEvent&)e;
			scale -= l.getYOffset() * 0.05f * scale;
			cam.setProjection(aspectRatio, scale);
		}
	}

	virtual void onExit() override
	{

	}



private:
	Ruby::Camera cam;
	float aspectRatio = 0.0f;
	float scale = 1.0f;

	void updateInputs()
	{
		glm::vec2 pos = cam.getPosition();
		pos.x += 0.03f * scale * (Ruby::Input::isKeyDown(Ruby::KeyCode::D) - Ruby::Input::isKeyDown(Ruby::KeyCode::A));
		pos.y += 0.03f * scale * (Ruby::Input::isKeyDown(Ruby::KeyCode::W) - Ruby::Input::isKeyDown(Ruby::KeyCode::S));
		cam.setPosition(pos);
	}
};

Ruby::App* createApp(int argc, char** argv)
{
	Ruby::AppSpec spec;
	spec.MainDirectory = "../../../../Sandbox";
	spec.WindowSpec.Name = "Sandbox App";
	spec.WindowSpec.Width = 1280;
	spec.WindowSpec.Height = 720;
	spec.WindowSpec.MinWidth = 640;
	spec.WindowSpec.MinHeight = 360;
	spec.WindowSpec.VSync = true;
	spec.WindowSpec.HasTitleBar = true;
	return new Sandbox(argc, argv, spec);
}