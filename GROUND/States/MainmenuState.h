#pragma once

#include "../Engine/system/States.hpp"
#include "../Game.h"
#include "../Engine/ECS.h"
#include "../Engine/audio/SoundDevice.h"
#include <imgui.h>

class MainmenuState : public gr::States
{
private:
    GameDataRef _data;
    bool _options;
    bool _audio_sett;
    bool _display_sett;
    bool _controls_sett;
    // Audio
    float _globalV = 0;
    int _deviceSelected = 0;
    std::vector<const char*> _deviceT;
    // Display
    bool _vsync, _fullscreen;
    int _antialiasing, _fps;
    // Controls
    int _sens_value;
    unsigned int Image;
    bool play_selected = false;
    mINI::INIStructure audio, graphic, key;

public:
    MainmenuState(GameDataRef data);

    void init() override;
	void update(float deltaTime) override;
	void draw() override;	
	void AfterDraw() override;
	void destroyGL() override;
};
