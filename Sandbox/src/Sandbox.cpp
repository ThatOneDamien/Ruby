#include <Ruby.h>
#include <ImGui/imgui.h>
#include <string>

class TestLayer : public Ruby::Layer
{
public:
	TestLayer() : Ruby::Layer("Test Layer")
	{
		shader = nullptr;
		vao = nullptr;
	}

	virtual ~TestLayer() {}

	virtual void onPush() override 
	{
		float vertices[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		shader = Ruby::Shader::create("res/shaders/default.glsl");
		vao = Ruby::VertexArray::createVAO();

		Ruby::VertexLayout layout;
		layout.push({ Ruby::LayoutType::Float, 2, false });

		auto vbo = Ruby::VertexBuffer::createVBO((const void*)vertices, sizeof(vertices));
		vbo->setLayout(layout);

		auto ibo = Ruby::IndexBuffer::createIBO(indices, 6);
		vao->pushVertexBuffer(vbo);
		vao->setIndexBuffer(ibo);


	}

	virtual void update() override 
	{
		Ruby::Renderer::renderIndices(vao, shader, glm::mat4(0.0f));
	}
	virtual void ImGuiRender() override 
	{
		ImGui::Begin("Test Window");
		ImGui::Text("Poop");
		ImGui::End();
	}
private:
	std::shared_ptr<Ruby::Shader> shader;
	std::shared_ptr<Ruby::VertexArray> vao;
};

class Sandbox : public Ruby::App
{
public:
	Sandbox(int argc, char** argv, const std::string& mainDir) 
		: Ruby::App(argc, argv, mainDir)
	{
		pushOverlay(new TestLayer());
	}

	virtual ~Sandbox()
	{

	}
};

RUBY_MAIN_FUNCTION_ENTRY(Sandbox, std::string("../Ruby"));