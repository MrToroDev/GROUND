#include "Console.h"
#include <imgui.h>
#include "GR_cross_definitions.h"

namespace gr
{
    Console::Console()
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        style->Colors[ImGuiCol_Text] = ImVec4(1.0, 1.0, 1.0, 1.0);
    }
    
    Console::~Console()
    {}
    
    void Console::Draw()
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

        ImGui::End();
    }
}