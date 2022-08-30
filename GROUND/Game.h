#pragma once

#include <memory>
#include <string>
#include <map>
#include "Engine/system/StateMachine.h"
#include "Engine/Console.h"
#include "Engine/ECS.h"
#include "Engine/window/Window.h"
#include "PhysXInitialization.h"
#include <mini/ini.h>
#include <glm/glm.hpp>
#include <DiscordSDK/discord.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

struct GameData {

	gr::Window* window;
	gr::Manager manager;
	gr::StateMachine machine;
	mINI::INIFile* graphic_settings;
	mINI::INIFile* audio_settings;
	mINI::INIFile* keys_settings;
	gr::Console* debugConsole;
	float FPS, CPU_MS, RENDER_MS;
	discord::Core* dsCore;

	PhysicWorld PhysWorld;
};

typedef std::shared_ptr<GameData> GameDataRef;

class Game
{
public:
	Game(std::string title);
	~Game();

private:
	GameDataRef _data = std::make_shared<GameData>();
	bool debugConsole;
	bool Initialize = true;
	mINI::INIStructure graphic;

	void CreateContext(std::string title);
	void initFile();
	void initWindow(std::string title);
	void initImGui();
	void initDiscord();

	void run();

	static void GLAPIENTRY DebugProc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
};

