#include "MainmenuState.h"
#include "GameState.h"
#include "../Engine/window/Keyboard.h"
#include "../Engine/graphics/GraphicLoader.h"

MainmenuState::MainmenuState(GameDataRef data)
    : _data(data)
{
}

void MainmenuState::init()
{
    _data->audio_settings->read(audio);
    _data->graphic_settings->read(graphic);
    _data->keys_settings->read(key);


    _options = false;
    _audio_sett = false;
    _display_sett = true;
    _controls_sett = false;

    for (auto e : gr::SoundDevice::GetDevices())
    {
        _deviceT.push_back(e);
    }

    _deviceSelected = std::stoi(audio["Settings"]["DEVICE"]);
    _globalV = std::stoi(audio["Settings"]["GLOBAL_VOLUME"]);
    _vsync = (bool)std::stoi(graphic["Graphic"]["VSYNC"]);
    _fullscreen = (bool)std::stoi(graphic["Graphic"]["FULLSCREEN"]);
    _antialiasing = std::stoi(graphic["Graphic"]["ANTIALIASING"]);
    _sens_value = std::stoi(key["View"]["sensitivity"]);
    _fps = std::stoi(graphic["Video"]["hz"]);

    Image = gr::LoadTexture2D("Assets/GFX/GROUND_APP.png");
}

void MainmenuState::update(float deltaTime)
{
    if (play_selected)
    {
        audio["Settings"]["DEVICE"] = std::to_string(_deviceSelected);
        audio["Settings"]["GLOBAL_VOLUME"] = std::to_string(_globalV);
        _data->audio_settings->write(audio);
        graphic["Graphic"]["VSYNC"] = std::to_string(_vsync);
        graphic["Graphic"]["FULLSCREEN"] = std::to_string(_fullscreen);
        graphic["Graphic"]["ANTIALIASING"] = std::to_string(_antialiasing);
        graphic["Video"]["hz"] = std::to_string(float(_fps));
        _data->graphic_settings->write(graphic);
        key["View"]["sensitivity"] = std::to_string(_sens_value);
        _data->keys_settings->write(key);
        _data->machine.AddState(gr::StatesRef(new GameState(_data)));
    }
}

void MainmenuState::draw()
{
    int flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse
        | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar;
    ImGui::Begin("MainMenu", (bool*)0, flags);
    ImGui::SetWindowSize(ImVec2(_data->window->GetWidth(), _data->window->GetHeight()));
    ImVec2 winSize = ImGui::GetWindowSize();

    if (play_selected) {
        ImGui::SetCursorPosY(winSize.y - 40);
        ImGui::Text("Loading...");
    }
    else {

        if (!_options)
        {
            ImGui::SetCursorPosX(winSize.x / 2 - 110);
            ImGui::SetWindowFontScale(4.0f);
            ImGui::Text("GROUND");
            ImGui::SetWindowFontScale(1);

            ImGui::SetCursorPos(ImVec2(winSize.x / 2 - 50, winSize.y / 2 - 50));
            if (ImGui::Button("Play", ImVec2(100, 50))) {
                play_selected = true;
            }

            ImGui::SetCursorPos(ImVec2(winSize.x / 2 - 50, winSize.y / 2 + 100));
            if (ImGui::Button("Options", ImVec2(100, 50))) {
                _options = true;
            }

            ImGui::SetCursorPos(ImVec2(winSize.x / 2 - 50, winSize.y / 2 + 250));
            if (ImGui::Button("Exit", ImVec2(100, 50))) {
                _data->window->Close();
            }
        }
        else {
            if (ImGui::Button("Display", ImVec2(100, 40)))
            {
                _display_sett = true;
                _audio_sett = false;
                _controls_sett = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Audio", ImVec2(100, 40)))
            {
                _display_sett = false;
                _audio_sett = true;
                _controls_sett = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Controls", ImVec2(100, 40)))
            {
                _display_sett = false;
                _audio_sett = false;
                _controls_sett = true;
            }
            ImGui::Separator();
            if (_audio_sett)
            {
                ImGui::ListBox("Audio Device", &_deviceSelected, _deviceT.data(), gr::SoundDevice::GetDevices().size());
                ImGui::Separator();
                ImGui::SliderFloat("Global Volume", &_globalV, 0, 100, std::to_string((int)_globalV).c_str(), 0);
            }
            if (_display_sett)
            {
                ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "Remember! All the display settings need to restart!");

                ImGui::SliderInt("FPS", &_fps, 15, 244);
                ImGui::SliderInt("Antialiasing", &_antialiasing, 0, 16, std::to_string(_antialiasing).c_str(), 0);
                ImGui::Checkbox("Vsync", &_vsync);
                ImGui::SameLine();
                ImGui::Checkbox("Fullscreen", &_fullscreen);
            }
            if (_controls_sett)
            {
                ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "Attention! When VSYNC is disabled, the sensitivity increase.");
                ImGui::SliderInt("Sensivity", &_sens_value, 0, 100, std::to_string(_sens_value).c_str(), 0);
            }

            ImGui::SetCursorPosY(winSize.y - 40);
            if (ImGui::Button("Back", ImVec2(50, 30))) {
                audio["Settings"]["DEVICE"] = std::to_string(_deviceSelected);
                audio["Settings"]["GLOBAL_VOLUME"] = std::to_string(_globalV);
                _data->audio_settings->write(audio);
                graphic["Graphic"]["VSYNC"] = std::to_string(_vsync);
                graphic["Graphic"]["FULLSCREEN"] = std::to_string(_fullscreen);
                graphic["Graphic"]["ANTIALIASING"] = std::to_string(_antialiasing);
                graphic["Video"]["hz"] = std::to_string(float(_fps));
                _data->graphic_settings->write(graphic);
                key["View"]["sensitivity"] = std::to_string(_sens_value);
                _data->keys_settings->write(key);
                _options = false;
            }
        }
        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::Image((ImTextureID)Image, ImVec2(winSize.x, winSize.y), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 0.2));
    }
    
    ImGui::End();
}

void MainmenuState::AfterDraw()
{
}

void MainmenuState::destroyGL()
{
}
