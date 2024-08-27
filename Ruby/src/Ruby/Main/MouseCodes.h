#pragma once

// From the github repository Hazel by The Cherno
// He copied the codes from glfw3.h but formatted them
// in an enum that looks really nice, so I stole it :)
#include <cstdint>

namespace Ruby 
{ 
    enum class MouseCode : uint16_t
    {
        // From glfw3.h
        Button0 = 0,
        Button1 = 1,
        Button2 = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,
        Button6 = 6,
        Button7 = 7,
    
        LeftClick    = Button0,
        RightClick   = Button1,
        ScrollButton = Button2,
        ButtonLast   = Button7
    };
}
