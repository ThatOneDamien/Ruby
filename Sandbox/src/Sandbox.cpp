#include <Ruby.h>
#include <Ruby/Main/MainFunctionEntry.h>
#include <string>
#include <ImGui/imgui.h>

class Sandbox : public Ruby::App
{
    template<typename T>
    using SP = Ruby::SharedPtr<T>;

    

public:
    Sandbox(int argc, char** argv, const Ruby::AppSpec& spec)
        : Ruby::App(argc, argv, spec)
    {
    }

    void Resizefunc(int width, int height)
    {

    }

    virtual ~Sandbox()
    {
        delete m_Mesh;
    }

    virtual void onStart() override
    {
        m_Mesh = new Ruby::Mesh("res/models/IronMan.obj");
        m_Cam.setViewDegrees({0.0f, 0.0f, 5.0f}, -90.0f, 0.0f);

        Ruby::Input::setMouseLocked(true);

        Ruby::Renderer3D::init();
        Ruby::Renderer3D::useCamera(m_Cam);
        Ruby::Renderer3D::setMesh(*m_Mesh);
    }

    virtual void update(double deltaMillis) override 
    {
        static size_t counter = 0;
        if(m_MouseLocked)
        {
            glm::vec3 translation{0.0f, 0.0f, 0.0f};
            translation.x = ((float)Ruby::Input::isKeyDown(Ruby::KeyCode::D) - (float)Ruby::Input::isKeyDown(Ruby::KeyCode::A)) * m_Speed;
            translation.y = ((float)Ruby::Input::isKeyDown(Ruby::KeyCode::Space) - (float)Ruby::Input::isKeyDown(Ruby::KeyCode::LeftShift)) * m_Speed;
            translation.z = ((float)Ruby::Input::isKeyDown(Ruby::KeyCode::W) - (float)Ruby::Input::isKeyDown(Ruby::KeyCode::S)) * m_Speed;
            if(m_UpdateCam || translation != glm::vec3{0.0})
            {
                m_Cam.axisTranslate(translation);
                m_UpdateCam = false;
            }
        }
        ++counter;
        if(counter >= 288)
        {
            counter = 0;
            Ruby::Renderer3D::reload();
        }
        Ruby::Context::clear(); 
        Ruby::Renderer3D::addInstance({0.0f, 0.0f, 0.0f}, {0.1f, 0.1f, 0.1f}, 0.0f, {0.0f, 0.0f, 1.0f});
        Ruby::Renderer3D::addInstance({20.0f, 0.0f, 0.0f}, {0.1f, 0.1f, 0.1f}, 0.0f, {0.0f, 0.0f, 1.0f});
        Ruby::Renderer3D::renderInstanced();

    }

    virtual void ImGuiRender(double deltaMillis) override
    {
        if(!m_MouseLocked && ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowSize({150.0f, 50.0f});
            ImGui::Text("FPS: %f", 1000.0f/deltaMillis);
            ImGui::End();
        }
    }

    virtual void onMouseMove(double xpos, double ypos) override
    {
        constexpr float sensitivity = 0.1f;

        if(m_MouseLocked && getWindow().isHovered())
        {
            if(m_GetMouse)
            {
                m_GetMouse = false;
                m_MouseLastX = xpos;
                m_MouseLastY = ypos;
            }
            float newYaw = m_Cam.getYaw() + (xpos - m_MouseLastX) * sensitivity;
            glm::mod(newYaw, 360.0f);
            float newPitch = m_Cam.getPitch() + (m_MouseLastY - ypos) * sensitivity;
            if(newPitch > 85.0f)
                newPitch = 85.0f;
            else if(newPitch < -85.0f)
                newPitch = -85.0f;
            m_Cam.setViewDegrees(m_Cam.getPosition(), newYaw, newPitch);
            m_UpdateCam = true;

            m_MouseLastX = xpos;
            m_MouseLastY = ypos;
        }
    }


    virtual void onWindowResize(uint16_t width, uint16_t height) override
    {
        Ruby::Context::setViewport(0, 0, (int)width, (int)height);
        m_Cam.setProjection(glm::radians(70.0f), (float)width / (float)height, 0.1f, 1000.0f);
    }

    virtual void onWindowChangeFocus(bool focused) override
    {
        if(focused)
            m_GetMouse = true;
    }

    virtual void onKeyEvent(Ruby::KeyCode code, int scancode, Ruby::KeyAction action, int mods) override
    {
        (void)scancode; (void)mods;
        if(action != Ruby::KeyAction::Pressed)
            return;

        if(code == Ruby::KeyCode::Escape)
        {
            m_MouseLocked = !m_MouseLocked;
            getWindow().setMouseLocked(m_MouseLocked);
            m_GetMouse = true;
        }
    }

    virtual void onMouseScroll(double xoff, double yoff) override
    {
        (void)xoff;
        if(!m_MouseLocked)
            return;
        m_Speed += 0.01f * yoff;
        if(m_Speed > 0.2f)
            m_Speed = 0.2f; 
        else if(m_Speed < 0.04f)
            m_Speed = 0.04f;

    }

    virtual void onExit() override
    {

    }


private:
    Ruby::PerspCamera m_Cam{glm::radians(70.0f), 1280.0f/720.0f, 0.1f, 1000.0f};
    Ruby::Mesh* m_Mesh;
    float m_Speed = 0.08f;
    bool m_GetMouse = true;
    bool m_MouseLocked = true;
    bool m_UpdateCam = false;
    double m_MouseLastX = 0.0, m_MouseLastY = 0.0;
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
