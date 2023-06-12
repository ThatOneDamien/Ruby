#include <Ruby.h>
#include <ImGui/imgui.h>
#include <string>

using namespace Ruby;

class TestLayer : public Layer
{
public:
	TestLayer() : Layer("Test Layer")
	{
	}

	virtual ~TestLayer() {}

	virtual void onPush() override 
	{
		tex = Texture::createTexture("res/images/poop.jpg");
	}

	virtual void update() override 
	{
		Renderer::resetBatch();

		/*for (int i = 0; i < 92; i++)
		{
			Ruby::Renderer::drawQuad({ 2.0f/92.0 * i - 1.0, -0.25f }, { 2.0f / 92.0, 0.5}, {i/92.0f, 1.0-i/92.0f, 0.4f, 1.0f});
		}*/
		Renderer::drawTexture({ -0.5f, -0.5f }, { 1.0f, 1.0f }, tex);

		Renderer::renderBatched();		  
		
	}
	virtual void ImGuiRender() override 
	{
		ImGui::Begin("Test Window");
		ImGui::Text("Poop");
		ImGui::End();
	}
private:
	std::shared_ptr<Texture> tex;
};

class Sandbox : public App
{
public:
	Sandbox(int argc, char** argv, const std::string& mainDir) 
		: App(argc, argv, mainDir)
	{
		pushOverlay(new TestLayer());
	}

	virtual ~Sandbox()
	{

	}
};

RUBY_MAIN_FUNCTION_ENTRY(Sandbox, std::string("../Ruby"))