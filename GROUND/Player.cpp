#include "Player.h"
#include "Definitions.h"
#include "Engine/window/Keyboard.h"
#include "Engine/window/Mouse.h"
#include "Engine/system/Collision.h"
#include "Engine/audio/Listener.h"
#include "Engine/GR_cross_definitions.h"

Player::Player(gr::Manager& m, mINI::INIFile keys)
{
    keys.read(_keys);
}

Player::~Player()
{
    delete camera;
    for (auto r : rays) {
        delete r;
    }
}

void Player::Init()
{
    camera = new gr::Camera(glm::vec3(0, 0, 3.f));
    this->lightColor = glm::vec3(1.0);
    speed = 2;
}

void Player::Update(float deltaTime, gr::Window* window)
{

    if (gr::Keyboard::IsKeyPressed(window, (gr::Keyboard::Key)std::stoi(_keys["Keyboard"]["MOVE_LEFT"]))) {
        this->camera->ProcessKeyboard(gr::Camera_Movement::LEFT, speed * deltaTime);
    }
    else if (gr::Keyboard::IsKeyPressed(window, (gr::Keyboard::Key)std::stoi(_keys["Keyboard"]["MOVE_RIGHT"]))) {
        this->camera->ProcessKeyboard(gr::Camera_Movement::RIGHT, speed * deltaTime);
    }
    else if (gr::Keyboard::IsKeyPressed(window, (gr::Keyboard::Key)std::stoi(_keys["Keyboard"]["MOVE_UP"]))) {
        this->camera->ProcessKeyboard(gr::Camera_Movement::FORWARD, speed * deltaTime);
    }
    else if (gr::Keyboard::IsKeyPressed(window, (gr::Keyboard::Key)std::stoi(_keys["Keyboard"]["MOVE_DOWN"]))) {
        this->camera->ProcessKeyboard(gr::Camera_Movement::BACKWARD, speed * deltaTime);
    }

    if (gr::Keyboard::IsKeyPressed(window, (gr::Keyboard::Key)std::stoi(_keys["Keyboard"]["LOOK_LEFT"]))) {
        this->camera->ProcessMouseMovement(-0.5 * std::stoi(_keys["View"]["sensitivity"]) * 5, 0);
    }
    if (gr::Keyboard::IsKeyPressed(window, (gr::Keyboard::Key)std::stoi(_keys["Keyboard"]["LOOK_RIGHT"]))) {
        this->camera->ProcessMouseMovement(0.5 * std::stoi(_keys["View"]["sensitivity"]) * 5, 0);
    }

    if (gr::Mouse::IsMouseButtonPressed(window, gr::Mouse::MouseButton::LEFT)) {
        rays.push_back(new gr::Ray(camera->Position, (camera->Position + camera->Front), GetShadingPath("Ray.vert"), GetShadingPath("Ray.frag")));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (m_joystick.IsConnected(gr::JoystickPlayers::Joystick1)) {
        if (m_joystick.GetAxesValue(gr::JoystickAxes::LEFT_STICK_Y) == 1) {
            this->camera->ProcessKeyboard(gr::Camera_Movement::BACKWARD, speed * deltaTime);
        }
        else if (m_joystick.GetAxesValue(gr::JoystickAxes::LEFT_STICK_Y) == -1) {
            this->camera->ProcessKeyboard(gr::Camera_Movement::FORWARD, speed * deltaTime);
        }
        
        if (m_joystick.GetAxesValue(gr::JoystickAxes::LEFT_STICK_X) == 1) {
            this->camera->ProcessKeyboard(gr::Camera_Movement::RIGHT, speed * deltaTime);
        }
        else if (m_joystick.GetAxesValue(gr::JoystickAxes::LEFT_STICK_X) == -1) {
            this->camera->ProcessKeyboard(gr::Camera_Movement::LEFT, speed * deltaTime);
        }

        if (m_joystick.IsButtonPressed(gr::JoystickButtons::LEFT_TRIGGER_2)) {
            this->camera->ProcessMouseMovement(-0.5 * std::stoi(_keys["View"]["sensitivity"]) * 5, 0);
        }
        else if (m_joystick.IsButtonPressed(gr::JoystickButtons::RIGHT_TRIGGER_2)) {
            this->camera->ProcessMouseMovement(0.5 * std::stoi(_keys["View"]["sensitivity"]) * 5, 0);
        }

        if (m_joystick.IsButtonPressed(gr::JoystickButtons::A)) {
            rays.push_back(new gr::Ray(camera->Position, (camera->Position + camera->Front), GetShadingPath("Ray.vert"), GetShadingPath("Ray.frag")));
        }
    }

    gr::Listener::SetPosition(
        this->camera->Position.x,
        this->camera->Position.y,
        this->camera->Position.z
    );
}

void Player::UpdateDraw()
{
    for (auto e : rays) {
        e->SetViewProjection(
            GetProjection(this->WindowSize.x, this->WindowSize.y),
            GetView()
        );

        e->Draw(glm::vec3(1, 0, 0));
    }
}

void Player::OnCollision(gr::Entity* e)
{
    if (gr::Collision::AABB(GetTransform(), e)) {
        speed = -2;
    }
    else { speed = 2; }

}

glm::mat4 Player::GetProjection(float w, float y) const
{
    return glm::perspective(glm::radians(45.f), w / y, 0.1f, 1000.f);
}

glm::mat4 Player::GetView() const
{
    return camera->GetViewMatrix();
}

gr::Camera* Player::GetCamera() const
{
    return camera;
}

gr::TransformComponent* Player::GetTransform() const
{
    return new gr::TransformComponent(camera->Position.x, camera->Position.y, camera->Position.z, glm::vec3(1.0, 1.0, 1.0));
}