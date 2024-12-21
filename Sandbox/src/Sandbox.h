#include <Ruby.h>

class Sandbox : public Ruby::App
{
    template <typename T>
    using SP = Ruby::SharedPtr<T>;

public:
    Sandbox(int argc, char** argv, const Ruby::AppSpec& spec);
    virtual ~Sandbox() override;

    virtual void onStart() override;
    virtual void update(double deltaMillis) override;
    virtual void ImGuiRender(double deltaMillis) override;
    virtual void onMouseMove(double xpos, double ypos) override;
    virtual void onWindowResize(uint16_t width, uint16_t height) override;
    virtual void onWindowChangeFocus(bool focused) override;
    virtual void onKeyEvent(Ruby::KeyCode code, int scancode, Ruby::KeyAction action, int mods) override;
    virtual void onMouseScroll(double xoff, double yoff) override;
    virtual void onExit() override;
};
