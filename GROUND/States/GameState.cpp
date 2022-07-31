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

GameState::GameState(GameDataRef data)
    : _data(data) {}

void GameState::init()
{
    gr::SoundDevice::OpenDevice(gr::SoundDevice::GetDevices().at(_data->audio_settings["DEVICE"]));

    player = new Player(_data->manager, _data->supported_keys);
    player->Init();
    
    frame = new gr::Framebuffer(_data->window->GetWidth(), _data->window->GetHeight(), GetShadingPath("Frame.vert"), GetShadingPath("Frame.frag"));

    totalTimeText = new gr::Text(glm::vec2(_data->window->GetWidth(), _data->window->GetHeight()), glm::vec2(0, 0), "lorem ipsum", "Core/fonts/arcade_classic.ttf", 0.25f);
    totalTimeText->SetColor(gr::Color(1, 1, 1));
    fpsText = new gr::Text(glm::vec2(_data->window->GetWidth(), _data->window->GetHeight()), glm::vec2(0, 10), "lorem ipsum", "Core/fonts/arcade_classic.ttf", 0.25f);
    fpsText->SetColor(gr::Color(1, 1, 1));

    e = &_data->manager.addEntity();
    e->addComponent<gr::TransformComponent>(0, 0, 0, glm::vec3(1), 0);
    e->getComponent<gr::TransformComponent>().angleAxis = glm::vec3(0, 0, 1);
    e->addComponent<gr::Basic3DGeometry>(gr::Basic3DGeometryShapes::CUBE, GetShadingPath("Object3D.frag"), GetShadingPath("lightShader.vert"));
    
    b = new gr::Billboard(GetShadingPath("lightShader.vert"), GetShadingPath("Plane.frag"), "Core/GFX/tree.png");

    gr::Log("Loaded everything!");

    gr::Mouse::setCursorVisibility(_data->window, gr::Mouse::SHOW);
}

void GameState::update(float deltaTime)
{
    e->getComponent<gr::TransformComponent>().angle += 0.005;

    player->Update(deltaTime, _data->window);

    frame->GetShader()->use();
    frame->GetShader()->setFloat("_clock", deltaTime);
    

    std::stringstream ss, ss2;
    ss << "total: " << _data->CPU_MS;
    ss2 << "FPS: " << (int)_data->FPS;
    this->totalTimeText->SetText(ss.str());
    this->fpsText->SetText(ss2.str());
}

void GameState::draw()
{
    this->player->WindowSize = glm::vec2(_data->window->GetWidth(), _data->window->GetHeight());

    frame->Resize(_data->window->GetWidth(), _data->window->GetHeight());

    frame->Bind();
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    e->getComponent<gr::Basic3DGeometry>().SetProjectionView(
        player->GetProjection(_data->window->GetWidth(), _data->window->GetHeight()),
        player->GetView()
    );

    e->getComponent<gr::Basic3DGeometry>().SetLightAttribute(
        player->lightColor, player->GetTransform()->position, player->GetTransform()->position, player->GetCamera()->Front
    );
}

void GameState::AfterDraw()
{
    // Draw Transparent things
    b->SetVP(
        player->GetProjection(_data->window->GetWidth(), _data->window->GetHeight()),
        player->GetView()
    );

    b->SetLightAttributes(player->GetCamera()->Front, player->GetTransform()->position, player->GetTransform()->position, player->lightColor);

    b->Draw(glm::vec3(1.0, 0.0, 1.0), player->GetCamera());

    frame->UnBind();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    frame->DrawStorage();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    totalTimeText->Draw();
    fpsText->Draw();
}

void GameState::destroyGL()
{
    delete this->player;
    delete frame;
    delete totalTimeText;
    delete fpsText;
}