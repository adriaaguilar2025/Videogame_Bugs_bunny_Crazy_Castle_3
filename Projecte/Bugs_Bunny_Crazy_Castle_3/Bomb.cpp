#include "Bomb.h"
#include <iostream>

void Bomb::init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp, BombState initialState, TileMap* tm, const glm::vec2& displ) {
	pos = position;
	size = sz;
	sprite = sp;
	active = true;
	type = ENTITY_BOMB;
	bState = initialState;
	map = tm;             //Guardamos la referencia al mapa
	tileMapDispl = displ; //Guardamos el desplazamiento

	//Si se acaba de soltar, ponemos la animación 1 (parpadeo)
	if (sprite != NULL) {
		if (bState == DROPPED) sprite->changeAnimation(1);
		else sprite->changeAnimation(0);
	}
}

int Bomb::update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) {
	if (!active) return 0;

	if (sprite != NULL) sprite->update(deltaTime);

	//FÍSICAS DE GRAVEDAD
	pos.y += 4.0f; // Velocidad de caída
	int dummyY = pos.y;
	if (map != nullptr && map->collisionMoveDown(glm::ivec2(pos.x, pos.y), glm::ivec2(size.x, size.y), &dummyY)) {
		pos.y = dummyY; // Choca con el suelo y se detiene
	}
	// ------------------------------------

	//CASO 1: Está en el mapa para recogerse
	if (bState == COLLECTIBLE) {
		glm::vec2 pPos = player->getPosition();
		glm::vec2 pSize = glm::vec2(16, 32);

		if (checkAABBCollision(pPos, pSize, pos, size)) {
			active = false; // Desaparece del mapa
			player->addBomb(); // Suma al contador numérico del jugador
			std::cout << "Bomba recogida! Llevas: " << player->getBombs() << std::endl;
		}
	}
	//CASO 2: El jugador la soltó (está encendida)
	else if (bState == DROPPED) {
		// Comprueba colisión con entidades (enemigos)
		for (Entity* e : entities) {
			if (e->isActive() && e->getType() == ENTITY_ENEMY) {
				if (checkAABBCollision(pos, size, e->getPos(), e->getSize())) {
					e->die();      // Mata al enemigo
					active = false; // La bomba explota (desaparece)
					std::cout << "¡BOOM! Un enemigo ha explotado." << std::endl;
					break; // Solo afecta a 1 enemigo a la vez
				}
			}
		}
	}
	if (sprite) {
		sprite->setPosition(glm::vec2(tileMapDispl.x + pos.x, tileMapDispl.y + pos.y));
	}
	return 0;
}