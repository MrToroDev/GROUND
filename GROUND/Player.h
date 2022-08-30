#pragma once

#include "Engine/Camera.h"
#include "Engine/ECS.h"
#include "Engine/window/Window.h"
#include "Engine/window/Joystick.h"
#include "Engine/Components.h"
#include "Engine/graphics/Ray.h"
#include <mini/ini.h>
#include <glm/glm.hpp>
#include <map>

class Player
{
public:
    Player(gr::Manager& m, mINI::INIFile keys);
    ~Player();

    void Init();
    void Update(float deltatime, gr::Window* win);
    void UpdateDraw();
    void OnCollision(gr::Entity* e);

    glm::mat4 GetProjection(float w, float y) const;
    glm::mat4 GetView() const;
    gr::Camera* GetCamera() const;

    gr::TransformComponent* GetTransform() const;

    glm::vec3 lightColor;
    glm::ivec2 WindowSize;

private:
    std::vector<gr::Ray*> rays;
    float speed;
    gr::Camera* camera;
    mINI::INIStructure _keys;
    gr::Joystick m_joystick;
};