#include <Ruby.h>
#include <Ruby/Main/MainFunctionEntry.h>
#include <ImGui/imgui.h>
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
		auto& window = Ruby::App::getInstance().getWindow();
		aspectRatio = (float)window.getWidth() / (float)window.getHeight();
		cam.setProjection(aspectRatio);
		
		
		font = Ruby::createShared<Ruby::Font>("res/fonts/Roboto-Regular.ttf", 50);
		tex = font->getAtlasTexture();
		
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

		Ruby::Audio::play(*clip);
	}

	virtual void update(double deltaMillis) override 
	{
		updateInputs();
		Ruby::Renderer::API::clear();

		static double curSec = -800.0f;
		
		auto& info = font->getCharInfo(test);
		auto* texCoords = info.SubTex->getTexCoords();
		glm::vec2 size = { info.Size.x * 0.0025f, info.Size.y * 0.0025f };

		float vertices[] =
		{
			-size.x, -size.y, texCoords[3].x, texCoords[3].y,
			 size.x, -size.y, texCoords[2].x, texCoords[2].y,
			 size.x,  size.y, texCoords[1].x, texCoords[1].y,
			-size.x,  size.y, texCoords[0].x, texCoords[0].y,
		};

		vbo->bind();
		vbo->setVertexData(vertices, 64, 0);

		Ruby::Renderer::API::clear();
		tex->bind(0);
		Ruby::Renderer::renderSubmit(vao, shader);
		if (curSec >= 475.0f && test < 128)
		{
			while (!font->getCharSubTexture(++test) || test >= 128);
			curSec = 0.0f;
		}
		curSec += deltaMillis;
	}

	virtual void ImGuiRender() override
	{

	}

private:
	SP<Ruby::VertexArray> vao;
	SP<Ruby::VertexBuffer> vbo;
	SP<Ruby::IndexBuffer> ibo;
	SP<Ruby::Shader> shader;
	SP<Ruby::Font> font;
	SP<Ruby::Texture> tex;
	Ruby::Camera cam{1.0f};
	uint8_t test = '1';
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
		cam.setPosition({ pos.x + xDelt, pos.y + yDelt });
		shader->setUniformMat4("u_ViewProj", cam.getViewProjectionMatrix());
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