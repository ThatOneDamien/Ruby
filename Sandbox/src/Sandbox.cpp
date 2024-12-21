#include "Sandbox.h"
#include <Ruby/Main/MainFunctionEntry.h>

Sandbox::Sandbox(int argc, char** argv, const Ruby::AppSpec& spec) 
    : Ruby::App(argc, argv, spec) {}

Sandbox::~Sandbox() {}

void Sandbox::onStart()
{
    RB_TRACE("This is a sandbox app!!!");
}

void Sandbox::update(double deltaMillis) { }
void Sandbox::ImGuiRender(double deltaMillis) { }
void Sandbox::onMouseMove(double xpos, double ypos) { }
void Sandbox::onWindowResize(uint16_t width, uint16_t height) { }
void Sandbox::onWindowChangeFocus(bool focused) { }
void Sandbox::onKeyEvent(Ruby::KeyCode code, int scancode, Ruby::KeyAction action, int mods) { }
void Sandbox::onMouseScroll(double xoff, double yoff) { }
void Sandbox::onExit() { }

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
