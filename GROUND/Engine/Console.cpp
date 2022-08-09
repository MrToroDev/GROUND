#include "Console.h"
#include "GR_cross_definitions.h"
#include "window/Keyboard.h"

namespace gr
{
    Console::Console()
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        style->Colors[ImGuiCol_Text] = ImVec4(1.0, 1.0, 1.0, 1.0);
        buf = new char();
        user = malloc(100);
    }
    
    Console::~Console()
    {
        delete buf;
    }
    
    void Console::Draw(gr::Window* win)
    {
        ImGui::Begin("Debug Console");

        for (auto e : gr::GetLogBuffer()) {
            if (e.find("OK") != std::string::npos) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0, 1.0, 0.0, 1.0)); }
            else if (e.find("WARNING") != std::string::npos) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 0.0, 1.0)); }
            else if (e.find("ERROR") != std::string::npos) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.0, 0.0, 1.0)); }
            else { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 1.0, 1.0)); }
            ImGui::Text(e.c_str());
            ImGui::PopStyleColor();
        }
        if (gr::Keyboard::IsKeyPressed(win, gr::Keyboard::Key::ENTER)) { buf = new char(); }
        
        ImGui::InputText("lorem ipsum", buf, 100, 0, 0, user);
        if (user == "help" && gr::Keyboard::IsKeyPressed(win, gr::Keyboard::Key::ENTER)) { gr::Print("[USER_COMMANDS]", "help not found"); }
        ImGui::End();
    }
}