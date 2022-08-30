#pragma once

#include "window/Window.h"
#include <imgui.h>

namespace gr
{
    /**
     * @brief Base class for a console log gui with ImGui
     * 
     */
    class Console
    {
    public:
        Console();
        ~Console();

        void Draw(gr::Window* win);
    };
}