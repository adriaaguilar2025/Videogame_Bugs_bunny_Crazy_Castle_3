#include "Key.h"
#include <iostream>

void Key::init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp) {
	pos = position;
	size = sz;
	sprite = sp;
	active = true;
	actionValue = 0;
	type = ENTITY_KEY;
}

int Key::update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) {
	if (!active) return 0;

	glm::vec2 pPos = player->getPosition();
	glm::vec2 pSize = glm::vec2(16, 32);

	if (checkAABBCollision(pPos, pSize, pos, size)) {
		active = false;
		player->addKey();
		std::cout << "Llave conseguida! Llevas: " << player->getKeys() << std::endl;
	}
	return 0;
}