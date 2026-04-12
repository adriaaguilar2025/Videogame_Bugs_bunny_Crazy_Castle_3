#include "FinalDoor.h"
#include "Game.h"
#include <iostream>

void FinalDoor::init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp, int dest, int reqKeys) {
	Door::init(position, sz, sp, dest);
	type = ENTITY_FINAL_DOOR;
	requiredKeys = reqKeys; // Nos guardamos el precio de la puerta
}

int FinalDoor::update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) {
	if (!active) return 0;

	glm::vec2 pPos = player->getPosition();
	glm::vec2 feetPos = glm::vec2(pPos.x, pPos.y + 16.0f);
	glm::vec2 feetSize = glm::vec2(16, 16);

	if (checkAABBCollision(feetPos, feetSize, pos, size)) {
		if (Game::instance().getKey(GLFW_KEY_UP) && teleportCooldown <= 0.0f) {

			// EL NUEVO CONTROL DE SEGURIDAD: Miramos el inventario
			if (player->getKeys() < requiredKeys) {
				std::cout << "¡Puerta bloqueada! Necesitas " << requiredKeys << " llaves y llevas " << player->getKeys() << std::endl;
				return 0; // Denegado
			}
			else {
				setState(1); // ¡Se abre!
				std::cout << "¡Nivel completado!" << std::endl;
				return actionValue; // Pasamos de nivel
			}
		}
	}
	return 0;
}