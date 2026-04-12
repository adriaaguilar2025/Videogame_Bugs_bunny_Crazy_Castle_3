#include "Pato.h"
#include <iostream>

enum PatoAnims { MOVE_LEFT, MOVE_RIGHT, CLIMB, DIE };

Pato::Pato() {
    map = NULL;
    isDying = false;
    deathTimer = 0;
    isClimbing = false;
}

void Pato::init(const glm::vec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) {
    movingRight = true;
    speed = 1.0f; // Un pelín más rápido para que sea "chetado"
    map = tileMap;
    type = ENTITY_ENEMY;
    size = glm::vec2(16, 32);
    active = true;

    spritesheet.loadFromFile("images/enemies_sprite_sheet.png", TEXTURE_PIXEL_FORMAT_RGBA);

    float frameWidth = 1.0f / 22.0f;
    float frameHeight = 1.0f / 17.0f;

    sprite = Sprite::createSprite(size, glm::vec2(frameWidth, frameHeight), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(4);

    float FILA_PATO_1 = 0.0f;
    float FILA_PATO_2 = 1.0f;

    // Animaciones
    sprite->setAnimationSpeed(MOVE_LEFT, 8);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(frameWidth * 4.0f, frameHeight * FILA_PATO_1));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(frameWidth * 3.0f, frameHeight * FILA_PATO_1));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(frameWidth * 5.0f, frameHeight * FILA_PATO_1));

    sprite->setAnimationSpeed(MOVE_RIGHT, 8);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(frameWidth * 1.0f, frameHeight * FILA_PATO_1));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(frameWidth * 0.0f, frameHeight * FILA_PATO_1));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(frameWidth * 2.0f, frameHeight * FILA_PATO_1));

    sprite->setAnimationSpeed(CLIMB, 6);
    sprite->addKeyframe(CLIMB, glm::vec2(frameWidth * 0.0f, frameHeight * FILA_PATO_2));
    sprite->addKeyframe(CLIMB, glm::vec2(frameWidth * 1.0f, frameHeight * FILA_PATO_2));

    sprite->setAnimationSpeed(DIE, 3);
    sprite->addKeyframe(DIE, glm::vec2(frameWidth * 5.0f, frameHeight * FILA_PATO_2));
    sprite->addKeyframe(DIE, glm::vec2(frameWidth * 6.0f, frameHeight * FILA_PATO_2));
    sprite->addKeyframe(DIE, glm::vec2(frameWidth * 7.0f, frameHeight * FILA_PATO_2));

    sprite->changeAnimation(MOVE_RIGHT);
    tileMapDispl = tileMapPos;
}

int Pato::update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities)
{
	if (!active) return 0;
	if (player->isEnemiesFrozen()) return 0;

	if (isDying) {
		deathTimer += deltaTime;
		if (sprite) sprite->update(deltaTime);
		if (deathTimer > 1500) active = false;
		if (sprite) sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
		return 0;
	}

	sprite->update(deltaTime);
	glm::vec2 pPos = player->getPosition();

	// 1. DETECCIÓN DE LIANA (Láser en los pies)
	bool vineFeet = map->isOnStairs(glm::ivec2(pos.x + 8, pos.y + 31), glm::ivec2(1, 1));

	isClimbing = false;

	// --- 2. INTELIGENCIA DE TREPAR ---
	if (vineFeet) {
		// EL FIX DEL BLOQUE EXTRA: Bajamos la detección a 4.0f de nuevo. 
		// Ahora subirá hasta estar literalmente en la misma altura que Bugs Bunny.
		if (pPos.y < pos.y - 4.0f) {
			pos.x = ((int)(pos.x + 8) / 16) * 16;
			pos.y -= speed;
			isClimbing = true;
		}
		else if (pPos.y > pos.y + 4.0f) {
			pos.x = ((int)(pos.x + 8) / 16) * 16;
			pos.y += speed;
			isClimbing = true;

			int dummyY = pos.y;
			if (map->collisionMoveDown(glm::ivec2(pos.x, pos.y), glm::ivec2(16, 32), &dummyY)) {
				pos.y = dummyY;
				isClimbing = false;
			}
		}
		else {
			// SNAP MAGNÉTICO: Ya hemos subido el bloque extra y alcanzado a Bugs.
			// Alineamos la "Y" de Lucas a la cuadrícula exacta (múltiplo de 16) 
			// para que no haya colisiones fantasma al caminar por el pasillo.
			pos.y = ((int)(pos.y + 8.0f) / 16) * 16.0f;
			isClimbing = false;
		}
	}

	// --- 3. GRAVEDAD ---
	if (!isClimbing) {
		pos.y += 4.0f;
		int dummyY = pos.y;
		if (map->collisionMoveDown(glm::ivec2(pos.x, pos.y), glm::ivec2(16, 32), &dummyY) ||
			map->collisionMoveDownPiolin(glm::ivec2(pos.x, pos.y), glm::ivec2(16, 32), &dummyY)) {
			pos.y = dummyY;
		}
	}

	// --- 4. RADAR Y PERSECUCIÓN HORIZONTAL ---
	if (!isClimbing) {
		float targetX = pPos.x;

		if (std::abs(pPos.y - pos.y) > 16.0f) {
			int currentTileX = (pos.x + 8) / 16;
			int maxTiles = map->getMapSize().x;

			for (int i = 0; i < 20; i++) {
				if (currentTileX + i < maxTiles) {
					bool vF = map->isOnStairs(glm::vec2((currentTileX + i) * 16 + 8, pos.y + 31), glm::ivec2(1, 1));
					if (vF) { targetX = (currentTileX + i) * 16; break; }
				}
				if (currentTileX - i >= 0) {
					bool vF = map->isOnStairs(glm::vec2((currentTileX - i) * 16 + 8, pos.y + 31), glm::ivec2(1, 1));
					if (vF) { targetX = (currentTileX - i) * 16; break; }
				}
			}
		}

		// Recortamos la caja lateral a 14 de alto para limpiar rasguños con el suelo
		if (targetX < pos.x - 1.0f) {
			pos.x -= speed;
			movingRight = false;
			if (map->collisionMoveLeft(glm::ivec2(pos.x, pos.y + 16), glm::ivec2(16, 14))) pos.x += speed;
		}
		else if (targetX > pos.x + 1.0f) {
			pos.x += speed;
			movingRight = true;
			if (map->collisionMoveRight(glm::ivec2(pos.x, pos.y + 16), glm::ivec2(16, 14))) pos.x -= speed;
		}
	}

	// --- 5. ACTUALIZAR ANIMACIONES ---
	if (isClimbing) {
		if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
	}
	else {
		if (movingRight) {
			if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
		}
		else {
			if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));

	// --- 6. COLISIÓN CON EL JUGADOR ---
	glm::vec2 pSize = glm::vec2(16, 32);
	if (checkAABBCollision(pPos, pSize, pos, size)) {
		player->die(numvidasmenos);
	}

	return 0;
}

// ... resto del código de update ...

void Pato::die() {
    if (isDying) return;
    isDying = true;
    deathTimer = 0;
    if (sprite) sprite->changeAnimation(DIE);
}

// BORRA cualquier llave extra que haya aquí abajo si no hay un namespace