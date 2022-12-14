#include "Window.h"
#include <string>

namespace gr
{
    Window::Window(int width, int height, const char* title, WindowSettings settings)
    {
        GR_ASSERT(glfwInit());
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.majorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.minorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, (settings.CoreProfile) ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_ANY_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, settings.antialiasing);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        if (settings.Fullscreen)
        {
            _window = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
        }
        else
        {
            _window = glfwCreateWindow(width, height, title, NULL, NULL);
        }
        GR_ASSERT(_window);
        glfwSetWindowAttrib(_window, GLFW_RESIZABLE, settings.Resizable);
        glfwMakeContextCurrent(_window);
        if (settings.Vsync)
        {
            glfwSwapInterval(1);
        }
        else {
            glfwSwapInterval(0);
        }

        _width = width;
        _height = height;
    }
    
    Window::~Window()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }
    
    void Window::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    
    void Window::Display()
    {
        glfwSwapBuffers(_window);
    }
    
    void Window::PollEvents()
    {
        glfwPollEvents();
        glfwGetWindowSize(_window, &_width, &_height);
    }
    
    void Window::Close()
    {
        glfwSetWindowShouldClose(_window, GLFW_TRUE);
    }
    
    bool Window::IsOpen()
    {
        return !glfwWindowShouldClose(_window);
    }

    bool Window::isFocusing()
    {
        return glfwGetWindowAttrib(this->_window, GLFW_FOCUSED);
    }
    
    int Window::GetWidth()
    {
        return _width;
    }
    
    int Window::GetHeight()
    {
        return _height;       
    }
    
    GLFWwindow* Window::GetHandle()
    {
        return _window;
    }
}