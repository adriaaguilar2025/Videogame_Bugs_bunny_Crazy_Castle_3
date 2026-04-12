#include "Weight.h"
#include "Game.h"

void Weight::init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp, TileMap* tm, const glm::vec2& displ) {
	pos = position;
	size = sz;
	sprite = sp;
	active = true;
	type = ENTITY_WEIGHT;
	map = tm;
	tileMapDispl = displ;
}

int Weight::update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) {
	if (!active) return 0;

	// 0. GUARDAMOS LA POSICIÓN INICIAL
	glm::vec2 oldPos = pos;

	// 1. FÍSICAS: GRAVEDAD DE LA PESA
	pos.y += 4.0f;
	int dummyY = pos.y;
	if (map->collisionMoveDown(glm::ivec2(pos.x, pos.y), glm::ivec2(size.x, size.y), &dummyY)) {
		pos.y = dummyY;
	}

	// 2. LÓGICA DE EMPUJE (Bugs Bunny moviéndola)
	glm::vec2 pPos = player->getPosition();
	glm::vec2 pSize = glm::vec2(16, 32);

	if (checkAABBCollision(pPos, pSize, pos, size)) {
		if (Game::instance().getKey(GLFW_KEY_RIGHT) && pPos.x < pos.x) {
			pos.x += 2.0f;
			if (map->collisionMoveRight(glm::ivec2(pos.x, pos.y), glm::ivec2(size.x, size.y))) pos.x -= 2.0f;
		}
		else if (Game::instance().getKey(GLFW_KEY_LEFT) && pPos.x > pos.x) {
			pos.x -= 2.0f;
			if (map->collisionMoveLeft(glm::ivec2(pos.x, pos.y), glm::ivec2(size.x, size.y))) pos.x += 2.0f;
		}
	}

	// 3. ¿SE HA MOVIDO ESTE FRAME? (Cayendo o empujada)
	bool isMoving = (pos.x != oldPos.x || pos.y != oldPos.y);

	// 4. LÓGICA DE APLASTAR ENEMIGOS (¡Solo si está en movimiento!)
	if (isMoving) {
		for (size_t i = 0; i < entities.size(); i++) {
			if (entities[i]->getType() == ENTITY_ENEMY && entities[i]->isActive()) {
				glm::vec2 enemyPos = entities[i]->getPos();
				glm::vec2 enemySize = entities[i]->getSize(); // Usamos nuestro nuevo método

				if (checkAABBCollision(pos, size, enemyPos, enemySize)) {
					entities[i]->die();
				}
			}
		}
	}

	// 5. ACTUALIZAR POSICIÓN VISUAL
	if (sprite) {
		sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
	}

	return 0;
}