#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include "Game.h"
#include "Engine/graphics/GraphicLoader.h"
#include "States/MainmenuState.h"
#include "Engine/graphics/Shader.h"
#include "Definitions.h"
#include "Engine/system/DeviceInfo.h"
#include "Engine/GR_cross_definitions.h"
#include "Engine/audio/SoundDevice.h"
#include "Engine/window/Keyboard.h"
#include "Engine/utils/DateTime.h"
#include "Engine/system/Memory.h"

Game::Game(std::string title)
{
	try
	{
		CreateContext(title);
	}
	catch (std::exception &e)
	{
		std::cout << "ERROR! something went wrong!" << std::endl
				  << "Error: " << e.what() << std::endl;
		ERROR_MESSAGE("GROUND", (LPCSTR)GR_TO_CSTRING("Error: ", e.what()));
		{
			std::ofstream logFile;
			std::string file = "logs/log-" + gr::Time::GetDateTime() + ".log";
			logFile.open(file, std::ios::out);
			for (auto t : gr::GetLogBuffer()) {
				logFile << t << std::endl;
			}

			logFile.close();
		}
		__debugbreak();
		return;
	}
}

Game::~Game()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	{
		std::ofstream logFile;
		std::string file = "logs/log-" + gr::Time::GetDateTime() + ".log";
		logFile.open(file, std::ios::out);
		for (auto t : gr::GetLogBuffer()) {
			logFile << t << std::endl;
		}

		logFile.close();
	}

	this->_data->manager.destroyGL();
	this->_data->machine.GetActiveState()->destroyGL();
	gr::Shader::DeletePredefinedShader();
	gr::SoundDevice::CloseDevice();
	Sleep(10);
	delete _data->window;
	delete _data->debugConsole;
	delete _data->dsCore;

	delete _data->audio_settings;
	delete _data->graphic_settings;
	delete _data->keys_settings;
}

void Game::CreateContext(std::string title)
{
	if(Initialize) {
		this->initFile();
		this->initDiscord();
		this->initWindow(title);
		gr::InitOpenGL();
		this->initImGui();
		debugConsole = false;
		_data->machine.AddState(gr::StatesRef(new MainmenuState(_data)));
		_data->PhysWorld.Initialize(9.81f);
		Initialize = false;
		run();
	}
}

void Game::initFile()
{
	_data->graphic_settings = new mINI::INIFile("settings/Window.ini");
	_data->audio_settings = new mINI::INIFile("settings/Audio.ini");
	_data->keys_settings = new mINI::INIFile("settings/Keyboard.ini");
}

void Game::initWindow(std::string title)
{
	_data->graphic_settings->read(graphic);
	gr::WindowSettings settings;
	settings.antialiasing = std::stoi(graphic["Graphic"]["ANTIALIASING"]);
	settings.Vsync = (bool)std::stoi(graphic["Graphic"]["VSYNC"]);
	settings.Fullscreen = (bool)std::stoi(graphic["Graphic"]["FULLSCREEN"]);
	settings.Resizable = false;
#if _WIN32
	settings.majorVersion = 4;
	settings.minorVersion = 6;
	settings.CoreProfile = true;
#else
	settings.majorVersion = 2;
	settings.minorVersion = 1;
	settings.CoreProfile = false;
#endif

	_data->window = new gr::Window( 
		std::stoi(graphic["Video"]["width"], nullptr, 10), 
		std::stoi(graphic["Video"]["height"], nullptr, 10),
		title.c_str(), settings
	);
}

void Game::initImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.IniFilename = "settings/ImGui_Settings.ini";
	io.Fonts->AddFontFromFileTTF("Assets/fonts/arcade_classic.ttf", 10.0f);
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_data->window->GetHandle(), true);
#if _WIN32
	ImGui_ImplOpenGL3_Init("#version 460 core");
#else
	ImGui_ImplOpenGL3_Init("#version 120");
#endif
	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_Text] = ImVec4(1.0, 1.0, 1.0, 1.0);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 1);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(1, 0, 0, 1);
	style->Colors[ImGuiCol_Button] = ImVec4(0, 0.5, 0, 1);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 1, 0, 1);

	style->ScrollbarSize = 20;
	style->ScrollbarRounding = 0;
	_data->debugConsole = new gr::Console();
}

void Game::initDiscord()
{
	if (gr::Memory::IsProcessRunning(L"Discord.exe")) {
		auto result = discord::Core::Create(1007969098744995880, (uint64_t)discord::CreateFlags::NoRequireDiscord, &_data->dsCore);
		gr::Print("[Discord: Status]", "Running!");
		discord::Activity activity{};
		activity.SetType(discord::ActivityType::Playing);
		activity.SetDetails("Testing v0.0.5a");
		activity.GetAssets().SetLargeImage("ground_app");
		_data->dsCore->SetLogHook(discord::LogLevel::Debug, [](discord::LogLevel t, const char* text) { gr::Print("[Discord: Debug]", text); });
		_data->dsCore->SetLogHook(discord::LogLevel::Info, [](discord::LogLevel t, const char* text) { gr::Print("[Discord: Info]", text); });
		_data->dsCore->SetLogHook(discord::LogLevel::Warn, [](discord::LogLevel t, const char* text) { gr::LogWarning(text); });
		_data->dsCore->SetLogHook(discord::LogLevel::Error, [](discord::LogLevel t, const char* text) { gr::LogError(text); });
		_data->dsCore->ActivityManager().UpdateActivity(activity, [](discord::Result e) {});
	} else { gr::Print("[Discord: Status]", "Not Running!"); }
}

void Game::run()
{
	try {
		// Predefined Shader
		gr::Shader::CompilePredefinedShader(PREDEFINED_SHADER);
		float m_LastTime = 0, deltatime = 0;	
		double FPS = 0;
		std::mutex _mutex;

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugProc, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		while (_data->window->IsOpen())
		{
			if (gr::Memory::IsProcessRunning(L"Discord.exe")) {
				// Update Discord SDK Core
				_data->dsCore->RunCallbacks();
			}
			_data->window->PollEvents();

			this->_data->machine.ProcessChanges();

			if (this->_data->window->isFocusing()) {
				ImGui_ImplGlfw_NewFrame();
				ImGui_ImplOpenGL3_NewFrame();
				ImGui::NewFrame();

				// Update scene
				float time = float(glfwGetTime());
				deltatime = time - m_LastTime;

				float ftime = std::stof(graphic["Video"]["hz"]);

				if (deltatime >= (1.0 / ftime))
				{
					_data->FPS = 1.0 / deltatime;
					_data->CPU_MS = deltatime / FPS * 1000;

					m_LastTime = time;
					FPS = 0;

					this->_data->machine.GetActiveState()->update(deltatime);
					_data->manager.refresh();
					_data->manager.update(deltatime);

					_data->PhysWorld.mScene->simulate(deltatime);
					_data->PhysWorld.mScene->fetchResults(true);
				}

				/*----------Additional keybinds-------------*/
				if (gr::Keyboard::IsKeyPressed(_data->window, gr::Keyboard::Key::F5)) {
					debugConsole = true;
				}
				if (gr::Keyboard::IsKeyPressed(_data->window, gr::Keyboard::Key::ESCAPE)) {
					debugConsole = false;
				}
				/*------------------------------------------*/

				if (debugConsole)
				{
					_data->debugConsole->Draw(_data->window);
				}

				_data->window->Clear();

				auto startTime = std::chrono::high_resolution_clock::now();

				// Draw scene
				this->_data->machine.GetActiveState()->draw();
				this->_data->manager.draw();
				this->_data->machine.GetActiveState()->AfterDraw();

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				_data->window->Display();

				auto endTime = std::chrono::high_resolution_clock::now();
				auto startMicro = std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch().count();
				auto endMicro = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

				long long int duration = endMicro - startMicro;
				_data->RENDER_MS = duration * 0.001;

				FPS++;
			}
		}
	} catch (std::exception& e) {
		{
			std::ofstream logFile;
			std::string file = "logs/log-" + gr::Time::GetDateTime() + ".log";
			logFile.open(file, std::ios::out);
			for (auto t : gr::GetLogBuffer()) {
				logFile << t << std::endl;
			}

			logFile.close();
		}
		ERROR_MESSAGE("GROUND", (LPCSTR)GR_TO_CSTRING("Error: ", e.what()));
	}
}

void GLAPIENTRY Game::DebugProc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::stringstream ss;
	short color = 0;
	ss << "[Opengl: ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		ss << "DEBUG_SOURCE_API";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		ss << "DEBUG_SOURCE_APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		ss << "DEBUG_SOURCE_OTHER";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		ss << "DEBUG_SOURCE_SHADER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		ss << "DEBUG_SOURCE_THIRD-PARTY";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		ss << "DEBUG_SOURCE_WINDOWS";
		break;
	}
	switch (type)
	{
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		ss << "](DEPRECATED: ";
		color = 1;
		break;
	case GL_DEBUG_TYPE_ERROR:
		ss << "](ERROR: ";
		color = 2;
		break;
	case GL_DEBUG_TYPE_MARKER:
		ss << "](MARKER: ";
		color = 1;
		break;
	case GL_DEBUG_TYPE_OTHER:
		ss << "](OTHER: ";
		color = 1;
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		ss << "](PERFORMANCE: ";
		color = 1;
		break;

	default:
		ss << "](UNKNOWN[" << type << "]: ";
		color = 0;
	}
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		ss << "HIGH)";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		ss << "MEDIUM)";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		ss << "LOW)";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		ss << "NOTIFICATION)";
		break;
	}
	ss << " -> " << message;
	
	if (color == 1) { 
		gr::LogWarning(ss.str().c_str());
	}
	else if (color == 2) { 
		gr::LogError(ss.str().c_str()); 
	}
	else {
		gr::Print("[Debug::API]", ss.str().c_str()); 
	}
}
