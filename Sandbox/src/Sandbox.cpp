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

	virtual void onEvent(Event& e) override
	{
		switch (e.getType())
		{
		case EventType::KeyPressed:
			break;
		}
	}

	virtual void onPush() override 
	{
		tex = Texture::createTexture("res/images/poop.jpg");
		shader = Shader::createShader("res/shaders/default.glsl");
		vao = VertexArray::createVAO();

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

		auto vbo = VertexBuffer::createVBO(vertices, sizeof(vertices));
		for (int i = 0; i < 4; i++)
			vertices[i * 2] += 1.5f;
		auto vbo2 = VertexBuffer::createVBO(vertices, sizeof(vertices));
		auto ibo = IndexBuffer::createIBO(indices, 6);

		VertexLayout layout;
		layout.push({LayoutType::Float, 2, false});

		vbo->setLayout(layout);
		vbo2->setLayout(layout);

		vao->setIndexBuffer(ibo);
		vao->pushVertexBuffer(vbo);
		vao->pushVertexBuffer(vbo2);
		float a = 1280.0f / 720.0f * 2.0f;
		cam = std::make_unique<Camera>(-a, a, -2.0f, 2.0f);
	}

	virtual void update(double deltaSeconds) override 
	{
		const auto& a = vao->getVertexBufferList();
		cam->setPosition({ x, 0.0f });
		vao->bind();
		a[1]->bind();
		Renderer::renderSubmit(vao, shader, cam->getViewProjectionMatrix());
		/*vao->bind();
		a[1]->bind();
		Renderer::renderSubmit(vao, shader, cam->getViewProjectionMatrix());*/
		x += 0.8f * (float)deltaSeconds;
	}
	virtual void ImGuiRender() override 
	{
		ImGui::Begin("Test Window");
		ImGui::Text("Poop");
		ImGui::End();
	}
private:
	SharedPtr<Texture> tex;
	SharedPtr<Shader> shader;
	SharedPtr<VertexArray> vao;
	std::unique_ptr<Camera> cam;
	float x = 0.0f;
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