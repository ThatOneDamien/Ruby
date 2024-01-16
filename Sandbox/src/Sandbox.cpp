#include <Ruby.h>
#include <Ruby/Main/MainFunctionEntry.h>
#include <string>

class TestLayer : public Ruby::Layer
{
	template<typename T>
	using SP = Ruby::SharedPtr<T>;
public:
	TestLayer() : Ruby::Layer("Test Layer")
	{
	}

	virtual ~TestLayer() 
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

	virtual void onPush() override 
	{
		clip = Ruby::createShared<Ruby::AudioClip>("res/sounds/f.mp3");
		Ruby::Renderer::updateCamera(cam);
		aspectRatio = Ruby::App::getInstance().getWindow().getAspectRatio();

		Ruby::Audio::play3D(*clip, 0.0f, 0.0f);
	}

	virtual void update(double deltaMillis) override 
	{
		updateInputs();
		Ruby::Renderer::API::clear();
		Ruby::Renderer::drawText("I want to fuck my girl.", {-1.0, 0.0}, 0.01, {1.0, 1.0, 1.0, 1.0});
		Ruby::Renderer::drawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f});
		Ruby::Renderer::renderBatched();
		
	}

	virtual void ImGuiRender() override
	{

	}

private:
	Ruby::Camera cam;
	float aspectRatio = 0.0f;
	float scale = 1.0f;
	SP<Ruby::AudioClip> clip;

	void updateInputs()
	{
		float xDelt = 0, yDelt = 0;
		if (Ruby::Input::isKeyDown(Ruby::Key::A)) 
		{
			xDelt = -0.05 * scale;
		}
		else if(Ruby::Input::isKeyDown(Ruby::Key::D)) 
		{
			xDelt = 0.05 * scale;
		}
		if (Ruby::Input::isKeyDown(Ruby::Key::W)) 
		{
			yDelt = 0.05 * scale;
		}
		else if (Ruby::Input::isKeyDown(Ruby::Key::S)) 
		{
			yDelt = -0.05 * scale;
		}
		const glm::vec2& pos = cam.getPosition();
		Ruby::Audio::updateListener(pos.x, pos.y);
		cam.setPosition({ pos.x + xDelt, pos.y + yDelt });
	}
};

class Sandbox : public Ruby::App
{
public:
	Sandbox(int argc, char** argv, const std::string& mainDir, const char* name, int width, int height) 
		: App(argc, argv, mainDir, name, width, height)
	{
		pushOverlay(new TestLayer());
	}

	virtual ~Sandbox()
	{

	}
};

Ruby::App* createApp(int argc, char** argv)
{
	return new Sandbox(argc, argv, ".", "Sandbox App", 1280, 720);
}