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
        Ruby::Renderer::useCamera(cam);
        aspectRatio = Ruby::App::getInstance().getWindow().getAspectRatio();
        Ruby::Renderer::API::setClearColor({ 0.2f, 0.2f, 0.2f });
        Ruby::Entity e = scene.createEntity();
        Ruby::Components::Transform& t = e.addComponent<Ruby::Components::Transform>();
        t.Position = { 1.0f, 1.2f, 0.0f };
        t.Rotation = 1.0f;
        t.Scale = {1.0f, 0.3f};
        Ruby::Components::Sprite& s = e.addComponent<Ruby::Components::Sprite>();
        s.Color = { 0.3f, 0.3f, 0.4f, 1.0f };

        scene.serialize("bruh.rusc");
    }

    virtual void update(double deltaMillis) override 
    {
        updateInputs();
        Ruby::Renderer::API::clear();
        Ruby::Renderer::resetBatch();
        Ruby::Renderer::drawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f});
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
    Ruby::Scene scene{"Unnamed scene"};
    Ruby::Camera cam;
    float aspectRatio = 0.0f;
    float scale = 1.0f;

    void updateInputs()
    {
        glm::vec2 pos = cam.getPosition();
        pos.x += 0.03f * scale * (Ruby::Input::isKeyDown(Ruby::KeyCode::D) - Ruby::Input::isKeyDown(Ruby::KeyCode::A));
        pos.y += 0.03f * scale * (Ruby::Input::isKeyDown(Ruby::KeyCode::W) - Ruby::Input::isKeyDown(Ruby::KeyCode::S));
        cam.setPosition({pos.x, pos.y, 0.0f});
    }
};

Ruby::App* createApp(int argc, char** argv)
{
    Ruby::AppSpec spec;
    spec.MainDirectory = "../../../../Sandbox";
    spec.WinSpec.Name = "Sandbox App";
    spec.WinSpec.Width = 1280;
    spec.WinSpec.Height = 720;
    spec.WinSpec.MinWidth = 640;
    spec.WinSpec.MinHeight = 360;
    spec.WinSpec.VSync = true;
    spec.WinSpec.HasTitleBar = true;
    return new Sandbox(argc, argv, spec);
}
