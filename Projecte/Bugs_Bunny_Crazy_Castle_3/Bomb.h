#pragma once
#ifndef _BOMB_INCLUDE
#define _BOMB_INCLUDE
#include "Entity.h"
#include "TileMap.h"

class Bomb : public Entity {
public:
	enum BombState { COLLECTIBLE, DROPPED };

	void init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp, BombState initialState, TileMap* tm, const glm::vec2& displ);
	int update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) override;

	BombState getBombState() const { return bState; }

private:
	BombState bState;

	//Variables para la gravedad y posición visual
	TileMap* map;
	glm::vec2 tileMapDispl;
};
#endif