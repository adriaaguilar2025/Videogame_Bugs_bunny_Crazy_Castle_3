#pragma once
#ifndef _CLOCK_INCLUDE
#define _CLOCK_INCLUDE
#include "Entity.h"

class Clock : public Entity {
public:
	void init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp);
	int update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) override;
};
#endif