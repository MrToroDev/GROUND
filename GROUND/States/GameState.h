#pragma once

#include "../Engine/system/States.hpp"
#include "../Game.h"
#include <glm/glm.hpp>
#include "../Engine/ECS.h"
#include "../Player.h"
#include "../Engine/graphics/Framebuffer.h"
#include "../Engine/graphics/Text.h"
#include "../Engine/graphics/Billboard.h"
#include <chrono>

class GameState : public gr::States
{
private:
	GameDataRef _data;
	Player* player;
	gr::Framebuffer* frame;
	gr::Text* totalTimeText, * fpsText, * vramText;
	gr::Entity* cube, * test;
	gr::Billboard* b;

public:
	GameState(GameDataRef data);

	void init() override;
	void update(float deltaTime) override;
	void draw() override;	
	void AfterDraw() override;
	void destroyGL() override;
};