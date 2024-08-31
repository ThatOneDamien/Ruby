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
        m_VAO = Ruby::VertexArray::create();

        float vertices[] =
        {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
        };

        uint32_t indices[] =
        {
            0, 1, 2,
            2, 3, 0,

            1, 5, 6,
            6, 3, 1,

            5, 4, 7,
            7, 6, 5,

            4, 0, 3,
            3, 7, 4,

            0, 1, 5,
            5, 4, 0,

            3, 2, 6,
            6, 7, 3
        };

        m_VBO = Ruby::VertexBuffer::create(vertices, sizeof(float) * 24);
        Ruby::VertexLayout layout;
        layout.push(Ruby::LayoutType::Float, 3, false);
        m_VBO->setLayout(layout);
        m_IBO = Ruby::IndexBuffer::create(indices, 6 * 6);


        m_VAO->setVertexBuffer(m_VBO);
        m_VAO->setIndexBuffer(m_IBO);
        m_Shader = Ruby::Shader::create("res/shaders/random.glsl");
        m_Shader->bind();
        m_Shader->setUniformMat4("u_Proj", m_Cam.getViewProjectionMatrix());
    }

    virtual void update(double deltaMillis) override 
    {
        Ruby::Context::clear(); 
        Ruby::Context::drawCall(m_VAO); 
    }

    virtual void ImGuiRender(double deltaMillis) override
    {

    }

    virtual void onEvent(Ruby::Event& e) override
    {
        static uint32_t lastX = 0, lastY = 0;
        switch(e.getType())
        {
        case Ruby::EventType::MouseMove:
            Ruby::MouseMoveEvent& mme = (Ruby::MouseMoveEvent&)e;
            
        }
    }

    virtual void onExit() override
    {

    }



private:
    Ruby::PerspCamera m_Cam{glm::radians(45.0f), 1280.0f/720.0f, 0.1f, 100.0f};
    SP<Ruby::VertexArray> m_VAO;
    SP<Ruby::VertexBuffer> m_VBO;
    SP<Ruby::IndexBuffer> m_IBO;
    SP<Ruby::Shader> m_Shader;
};

Ruby::App* createApp(int argc, char** argv)
{
    Ruby::AppSpec spec;
    spec.MainDirectory = "../../../../Sandbox";
    spec.RubyDirectory = "../Ruby";
    spec.WinSpec.Name = "Sandbox App";
    spec.DesiredAPI = Ruby::API::OpenGL;
    spec.WinSpec.Width = 1280;
    spec.WinSpec.Height = 720;
    spec.WinSpec.MinWidth = 640;
    spec.WinSpec.MinHeight = 360;
    spec.WinSpec.VSync = true;
    spec.WinSpec.HasTitleBar = true;
    return new Sandbox(argc, argv, spec);
}
