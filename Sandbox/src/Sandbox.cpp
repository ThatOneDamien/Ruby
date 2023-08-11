#include <Ruby.h>
#include <Ruby/Main/MainFunctionEntry.h>
#include <ImGui/imgui.h>
#include <string>

class TestLayer : public Ruby::Layer
{
public:
	TestLayer() : Ruby::Layer("Test Layer")
	{
	}

	virtual ~TestLayer() {}

	virtual void onEvent(Ruby::Event& e) override
	{
	}

	virtual void onPush() override 
	{
		auto& window = Ruby::App::getInstance().getWindow();
		cam.setProjection((float)window.getWidth() / (float)window.getHeight());
		font = new Ruby::Font("res/fonts/Roboto-Regular.ttf", 70);
		shader = Ruby::Shader::createShader("res/shaders/text.glsl");
		vao = Ruby::VertexArray::createVAO();

		vbo = Ruby::VertexBuffer::createVBO(64);

		Ruby::VertexLayout layout;
		layout.push(Ruby::LayoutType::Float, 2, false);
		layout.push(Ruby::LayoutType::Float, 2, false);
		vbo->setLayout(layout);
		vao->setVertexBuffer(vbo);

		uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		ibo = Ruby::IndexBuffer::createIBO(indices, 6);
		vao->setIndexBuffer(ibo);

		shader->bind();
		shader->setUniformInt("u_Tex", 0);
		shader->setUniformMat4("u_ViewProj", cam.getViewProjectionMatrix());
	}

	virtual void update(double deltaMillis) override 
	{
		static double curSec = 0.0f;
		auto size = font->getTexture(test)->getSize() * 0.0025f;
		
		float vertices[] =
		{
			-size.x, -size.y, 0.0f, 1.0f,
			 size.x, -size.y, 1.0f, 1.0f,
			 size.x,  size.y, 1.0f, 0.0f,
			-size.x,  size.y, 0.0f, 0.0f
		};

		vbo->bind();
		vbo->setVertexData(vertices, 64, 0);

		Ruby::Renderer::API::clear();
		const auto& tex = font->getTexture(test);
		tex->bind(0);
		Ruby::Renderer::renderSubmit(vao, shader);
		if (curSec >= 10.0f && test < 128)
		{
			while (!font->getTexture(++test) && test < 128);
			curSec = 0.0f;
		}
		curSec += deltaMillis;
	}

	virtual void ImGuiRender() override
	{

	}

private:
	Ruby::SharedPtr<Ruby::VertexArray> vao;
	Ruby::SharedPtr<Ruby::VertexBuffer> vbo;
	Ruby::SharedPtr<Ruby::IndexBuffer> ibo;
	Ruby::SharedPtr<Ruby::Shader> shader;
	Ruby::Font* font{ nullptr };
	Ruby::Camera cam{1.0f};
	uint8_t test = '1';
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