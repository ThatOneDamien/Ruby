#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

#include "App.h"

namespace Ruby
{
    namespace Input
    {
        inline bool isKeyDown(KeyCode code)
        {
            return App::getInstance().getWindow().isKeyDown(code);
        }

        inline bool isMouseButtonDown(MouseCode code)
        {
            return App::getInstance().getWindow().isMouseButtonDown(code);
        }

        inline void getMousePosition(double* o_MouseX, double* o_MouseY)
        {
            return App::getInstance().getWindow().getCursorPos(o_MouseX, o_MouseY);
        }

        inline void setMouseLocked(bool locked)
        {
            App::getInstance().getWindow().setMouseLocked(locked);
        }
    }
}
