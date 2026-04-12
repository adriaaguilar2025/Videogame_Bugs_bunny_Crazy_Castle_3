#include "SpeedBoost.h"
#include <iostream>

void SpeedBoost::init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp) {
	pos = position;
	size = sz;
	sprite = sp;
	active = true;
	actionValue = 0;
	type = ENTITY_SPEED_BOOST;
}

int SpeedBoost::update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) {
	if (!active) return 0;

	glm::vec2 pPos = player->getPosition();
	// Asumimos el tamaño del jugador (igual que en Clock)
	glm::vec2 pSize = glm::vec2(16, 32);

	if (checkAABBCollision(pPos, pSize, pos, size)) {
		active = false; // El objeto desaparece al recogerlo
		player->activateSpeedBoost(); // Llamamos a la nueva función del jugador
		std::cout << "¡Velocidad x2 recogida!" << std::endl;
	}
	return 0;
}