#include "GameState.h"
#include <GL/GL.h>
#include "../Engine/graphics/GraphicLoader.h"
#include "../Engine/system/Vertex.h"
#include "../Engine/system/Color.h"
#include "../Engine/Components.h"
#include "../Engine/graphics/Shapes.h"
#include "../Engine/system/Collision.h"
#include "../Definitions.h"
#include "../Engine/audio/SoundSource.h"
#include "../Engine/audio/SoundBuffer.h"
#include "../Engine/audio/SoundDevice.h"
#include "../Engine/window/Mouse.h"
#include "../Engine/utils/Math.h"
#include "../Engine/system/DeviceInfo.h"
#include <imgui.h>
#include "../Engine/window/Keyboard.h"
#include "../Engine/system/Memory.h"

GameState::GameState(GameDataRef data)
    : _data(data) {}

void GameState::init()
{
    gr::SoundDevice::OpenDevice(gr::SoundDevice::GetDevices().at(_data->audio_settings["DEVICE"]));
      
    player = new Player(_data->manager, _data->supported_keys);
    player->Init();
    
    totalTimeText = new gr::Text(glm::vec2(_data->window->GetWidth(), _data->window->GetHeight()), glm::vec2(0, 0), "lorem ipsum", "Assets/fonts/arcade_classic.ttf", 0.25f);
    totalTimeText->SetColor(gr::Color(1, 1, 1));
    vramText = new gr::Text(glm::vec2(_data->window->GetWidth(), _data->window->GetHeight()), glm::vec2(0, 10), "lorem ipsum", "Assets/fonts/arcade_classic.ttf", 0.25f);
    vramText->SetColor(gr::Color(1, 1, 1));
    fpsText = new gr::Text(glm::vec2(_data->window->GetWidth(), _data->window->GetHeight()), glm::vec2(0, 20), "lorem ipsum", "Assets/fonts/arcade_classic.ttf", 0.25f);
    fpsText->SetColor(gr::Color(1, 1, 1));

    cube = &_data->manager.addEntity();
    cube->addComponent<gr::TransformComponent>(0, 0, 0, glm::vec3(1), 0);
    cube->addComponent<gr::Basic3DGeometry>(gr::Basic3DGeometryShapes::CUBE, GetShadingPath("Object3D.frag"), GetShadingPath("lightShader.vert"));

    test = &_data->manager.addEntity();
    test->addComponent<gr::TransformComponent>(-2, 0, -2, glm::vec3(/*0.0078125*/1), 0);
    std::vector<gr::Texture> t = {
        gr::Texture(gr::LoadTexture2D("Assets/GFX/test/brick_normal.jpg"), "normal"),
        gr::Texture(gr::LoadTexture2D("Assets/GFX/test/brick_diffuse.jpg"), "diffuse"),
        gr::Texture(gr::LoadTexture2D("Assets/GFX/test/brick_specular.jpg"), "specular"),
    };
    test->addComponent<gr::ModelComponent>("Assets/model/Sphere.fbx", t, GetShadingPath("Object3D.frag"), GetShadingPath("lightShader.vert"));
    
    b = new gr::Billboard(GetShadingPath("lightShader.vert"), GetShadingPath("Plane.frag"), "??");

    frame = new gr::Framebuffer(_data->window->GetWidth(), _data->window->GetHeight(), GetShadingPath("Frame.vert"), GetShadingPath("Frame.frag"));

    gr::Log("Loaded everything!");

    gr::Mouse::setCursorVisibility(_data->window, gr::Mouse::SHOW);
}

void GameState::update(float deltaTime)
{
    player->Update(deltaTime, _data->window);

    test->getComponent<gr::TransformComponent>().angleAxis = glm::vec3(0, 1, 0);
    test->getComponent<gr::TransformComponent>().angle += 0.05;

    std::stringstream ss, ss2, ss3;
    ss << "CPU:" << _data->CPU_MS;
    ss2 << "FPS:" << (int)_data->FPS;
    ss3 << "VRAM:" << gr::Memory::VRAMcurrentUsage() / GR_MEM_MB << " MB";
    this->totalTimeText->SetText(ss.str());
    this->fpsText->SetText(ss2.str());
    this->vramText->SetText(ss3.str());
}

void GameState::draw()
{
    this->player->WindowSize = glm::vec2(_data->window->GetWidth(), _data->window->GetHeight());

    frame->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);

    cube->getComponent<gr::Basic3DGeometry>().SetProjectionView(
        player->GetProjection(_data->window->GetWidth(), _data->window->GetHeight()),
        player->GetView()
    );

    cube->getComponent<gr::Basic3DGeometry>().SetLightAttribute(
        player->lightColor, player->GetTransform()->position, player->GetTransform()->position, player->GetCamera()->Front
    );

    b->SetVP(
        player->GetProjection(_data->window->GetWidth(), _data->window->GetHeight()),
        player->GetView()
    );

    test->getComponent<gr::ModelComponent>().SetProjectionView(
        player->GetProjection(_data->window->GetWidth(), _data->window->GetHeight()),
        player->GetView()
    );

    gr::Light l;
    l.cameraPos = player->GetTransform()->position;
    l.color = glm::vec3(1);
    l.front = player->GetCamera()->Front;
    l.position = player->GetTransform()->position;

    test->getComponent<gr::ModelComponent>().SetLight(l);
}

void GameState::AfterDraw()
{
    // Draw Transparent things
    b->Draw(glm::vec3(2, 0, 2), player->GetCamera());

    frame->UnBind();
    glDisable(GL_DEPTH_TEST);
    frame->DrawStorage();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    totalTimeText->Draw();
    fpsText->Draw();
    vramText->Draw();

    glDisable(GL_BLEND);
}

void GameState::destroyGL()
{
    delete this->player;
    delete frame;
    delete totalTimeText;
    delete fpsText;
    delete vramText;
}