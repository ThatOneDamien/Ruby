#include <Ruby.h>
#include <Ruby/Main/MainFunctionEntry.h>
#include <ImGui/imgui.h>
#include <string>

class TestLayer : public Ruby::Layer
{
public:
	TestLayer() : Ruby::Layer("Test Layer")
	{
		Ruby::Logger::getClientLogger()->basicLog("balls");
	}

	virtual ~TestLayer() {}

	virtual void onEvent(Ruby::Event& e) override
	{
	}

	virtual void onPush() override 
	{
		shader = Ruby::Shader::createShader("res/shaders/default.glsl");
		vao = Ruby::VertexArray::createVAO();

		float vertices[] =
		{
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};

		uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		auto vbo = Ruby::VertexBuffer::createVBO(vertices, sizeof(vertices));
		auto ibo = Ruby::IndexBuffer::createIBO(indices, 6);

		Ruby::VertexLayout layout;
		layout.push({ Ruby::LayoutType::Float, 2, false});

		vbo->setLayout(layout);

		vao->setIndexBuffer(ibo);
		vao->setVertexBuffer(vbo);
		float a = 1280.0f / 720.0f * 2.0f;
		cam = Ruby::createUniPtr<Ruby::Camera>(-a, a, -2.0f, 2.0f);
	}

	virtual void update(double deltaSeconds) override 
	{
		Ruby::Renderer::API::clear();
		Ruby::Renderer::resetBatch();
		Ruby::Renderer::renderBatched();
	}
	virtual void ImGuiRender() override 
	{
		ImGui::Begin("Test Window");
		ImGui::Text("Poop");
		ImGui::End();
	}
private:
	Ruby::SharedPtr<Ruby::Texture> tex;
	Ruby::SharedPtr<Ruby::Shader> shader;
	Ruby::SharedPtr<Ruby::VertexArray> vao;
	Ruby::UniPtr<Ruby::Camera> cam;
	//Ruby::Font* font{ nullptr };
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