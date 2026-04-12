#pragma once
#ifndef _SPEEDBOOST_INCLUDE
#define _SPEEDBOOST_INCLUDE
#include "Entity.h"

class SpeedBoost : public Entity {
public:
	void init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp);
	int update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) override;
};
#endif