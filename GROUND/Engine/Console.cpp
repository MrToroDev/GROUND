#include "Console.h"
#include "GR_cross_definitions.h"
#include "window/Keyboard.h"
#include <sstream>

namespace gr
{
	Console::Console()
	{
		
	}

	Console::~Console()
	{
	}

	void Console::Draw(gr::Window* win)
	{
		int flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar
			| ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar
			| ImGuiWindowFlags_::ImGuiWindowFlags_NoResize;
		ImGui::Begin("Debug Console", (bool*)0, flags);
		ImGui::SetWindowFocus();
		if (ImGui::BeginMenuBar()) {
			bool p;
			if (ImGui::MenuItem("Clear", NULL, &p)) {
				gr::GetLogBuffer().clear();
			}
			ImGui::EndMenuBar();
		}
		
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