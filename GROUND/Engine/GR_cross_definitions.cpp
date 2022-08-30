#include "GR_cross_definitions.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

namespace gr
{
    std::vector<std::string> __logs;

    void Log(const char* text)
    {
        #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 10);
        printf("[OK] %s \n", text);
        SetConsoleTextAttribute(hConsole, 7);
        #else
        printf("\033[0;34m[OK] %s \n", text);
        #endif

        __logs.push_back(GR_TO_CSTRING("[OK] ", text));
    }
    
    void LogError(const char* text)
    {
        #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 12);
        printf("[ERROR] %s \n", text);
        SetConsoleTextAttribute(hConsole, 7);
        #else
        printf("\031[0;31m[ERROR] %s \n", text);
        #endif

        __logs.push_back(GR_TO_CSTRING("[ERROR] ", text));
    }
    
    void LogWarning(const char* text)
    {
        #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 14);
        printf("[WARNING] %s \n", text);
        SetConsoleTextAttribute(hConsole, 7);
        #else
        printf("\033[0;33m[WARNING] %s \n", text);
        #endif

        __logs.push_back(GR_TO_CSTRING("[WARNING] ", text));
    }
    
    void Print(const char* caption, const char* text)
    {
        std::cout << caption << " " << text << std::endl;
        __logs.push_back(GR_TO_CSTRING(caption, std::string(" ").append(text)));
    }
    
    std::vector<std::string> &GetLogBuffer()
    {
        return __logs;
    }

    bool InitOpenGL()
    {
        #ifdef _WIN32
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LogError("Cannot load glad!");
            return false;
        }
        #else
        glewExperiment = true;
        GLenum error = glewInit();
        if (error != GLEW_OK) {
            LogError("Cannot load glew!");
            return false;
        }
        #endif

        return true;
    }
}