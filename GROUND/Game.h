#pragma once

#include <memory>
#include <string>
#include <map>
#include "Engine/system/StateMachine.h"
#include "Engine/utils/ConfigParser.h"
#include "Engine/Console.h"
#include "Engine/ECS.h"
#include "Engine/window/Window.h"
#include "PhysXInitialization.h"
#include <glm/glm.hpp>
#include <DiscordSDK/discord.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

struct GameData {
	gr::Window* window;
	gr::Manager manager;
	gr::StateMachine machine;
	gr::ConfigFile<int> supported_keys;
	gr::ConfigFile<int> graphics_settings;
	gr::ConfigFile<int> audio_settings;
	gr::Console* debugConsole;
	float FPS, CPU_MS;
	discord::Core* dsCore;

	PhysicWorld pWorld;
};

typedef std::shared_ptr<GameData> GameDataRef;

class Game
{
public:
	Game(std::string title);

private:
	GameDataRef _data = std::make_shared<GameData>();
	bool debugConsole;

	void initFile();
	void initWindow(std::string title);
	void initImGui();
	void initDiscord();

	void run();

	static void GLAPIENTRY DebugProc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
};

