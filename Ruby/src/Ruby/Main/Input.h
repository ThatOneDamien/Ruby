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
    }
}