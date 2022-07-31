// Define macros that have to be defined only one time
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <chrono>
#include <thread>
#include "Game.h"
#include "Definitions.h"

#ifdef _WIN32
#include <windows.h>

// Windows win main, if we run in windows
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nShowCmd
    )
#else
// common main for any other platforms
int main(int argc __attribute__((unused)), char** argv __attribute__((unused)))
#endif
{
    try {
        ::Game("GROUND");
    } catch(std::exception& e) {
        ERROR_MESSAGE("GROUND", e.what());
    }
    return 0;
}