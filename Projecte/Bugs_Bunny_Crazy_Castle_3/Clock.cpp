#include "Clock.h"
#include <iostream>

void Clock::init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp) {
	pos = position;
	size = sz;
	sprite = sp;
	active = true;
	actionValue = 0;
	type = ENTITY_CLOCK;
}

int Clock::update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) {
	if (!active) return 0;

	glm::vec2 pPos = player->getPosition();
	//Asumimos el tamaño del jugador
	glm::vec2 pSize = glm::vec2(16, 32);

	if (checkAABBCollision(pPos, pSize, pos, size)) {
		active = false; // El reloj desaparece del mapa
		player->addClock();
		std::cout << "Reloj recogido! Llevas: " << player->getClocks() << std::endl;
	}
	return 0;
}