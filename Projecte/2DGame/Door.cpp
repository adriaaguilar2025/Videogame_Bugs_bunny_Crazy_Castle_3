#include "Door.h"
#include "Game.h"

void Door::init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp, int dest) {
	pos = position; size = sz; sprite = sp; active = true; actionValue = dest;
	type = ENTITY_DOOR; state = 0;
}

// MAGIA: Al cambiar el estado, cambiamos la animación
void Door::setState(int s) {
	state = s;
	if (sprite != NULL) sprite->changeAnimation(state); // 0 = cerrada, 1 = abierta
}

int Door::update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) {
	if (!active) return 0;

	glm::vec2 pPos = player->getPosition();
	glm::vec2 feetPos = glm::vec2(pPos.x, pPos.y + 16.0f);
	glm::vec2 feetSize = glm::vec2(16, 16);

	if (checkAABBCollision(feetPos, feetSize, pos, size)) {
		if (Game::instance().getKey(GLFW_KEY_UP) && teleportCooldown <= 0.0f) {
			setState(1); // Marcamos la puerta como ABIERTA
			return actionValue;
		}
	}
	return 0;
}