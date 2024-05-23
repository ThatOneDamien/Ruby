#include <Ruby.h>
#include <Ruby/Main/MainFunctionEntry.h>
#include <string>

class Sandbox : public Ruby::App
{
	template<typename T>
	using SP = Ruby::SharedPtr<T>;
public:
	Sandbox(int argc, char** argv, const std::string& mainDir, const char* name, int width, int height)
		: App(argc, argv, mainDir, name, width, height)
	{
	}

	virtual ~Sandbox()
	{
	}

	virtual void onStart() override 
	{
		Ruby::Renderer::updateCamera(cam);
		aspectRatio = Ruby::App::getInstance().getWindow().getAspectRatio();
		Ruby::Renderer::API::setClearColor({1.0f, 1.0f, 1.0f});
		// scene = Ruby::createShared<Ruby::Scene>("blank");
		// Ruby::Entity entity = scene->createEntity();
		// entity.addComponent<Ruby::Components::Transform>(glm::vec2{0.0f, 0.0f}, 0.0f, glm::vec2{1.0f, 1.0f});
		// entity.addComponent<Ruby::Components::Sprite>(glm::vec4{1.0f, 1.0f, 0.0f, 1.0f});
	}

	virtual void update(double deltaMillis) override 
	{
		updateInputs();
		Ruby::Renderer::API::clear();
		Ruby::Renderer::resetBatch();
		Ruby::Renderer::drawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
		
		//Ruby::Renderer::drawText("Balls", { -0.5f, -0.5f }, 1.0f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
		//Ruby::Renderer::drawText("Balls", { -0.5f, 0.5f }, 1.0f, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f});
		//scene->updateScene(deltaMillis);
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
	Ruby::Camera cam{-1.0f, 1.0f, -1.0f, 1.0f};
	//SP<Ruby::Scene> scene;
	float aspectRatio = 0.0f;
	float scale = 1.0f;

	void updateInputs()
	{
		glm::vec2 pos = cam.getPosition();
		pos.x += 0.03f * scale * (Ruby::Input::isKeyDown(Ruby::Key::D) - Ruby::Input::isKeyDown(Ruby::Key::A));
		pos.y += 0.03f * scale * (Ruby::Input::isKeyDown(Ruby::Key::W) - Ruby::Input::isKeyDown(Ruby::Key::S));
		cam.setPosition(pos);
	}
};

Ruby::App* createApp(int argc, char** argv)
{
	return new Sandbox(argc, argv, "../../../../Sandbox", "Sandbox App", 1280, 720);
}