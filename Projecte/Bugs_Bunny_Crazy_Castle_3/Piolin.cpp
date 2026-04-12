#include "Piolin.h"
#include <iostream>

enum PiolinAnims
{
	MOVE_LEFT, MOVE_RIGHT, DIE // Añadimos DIE aquí
};

Piolin::Piolin()
{
	map = NULL;
	// Inicializamos las variables de muerte
	isDying = false;
	deathTimer = 0;
}

void Piolin::init(const glm::vec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap)
{
	movingRight = true;
	speed = 1;
	map = tileMap;
	type = ENTITY_ENEMY;

	// NOTA: 'pos' y 'size' y 'sprite' vienen de la clase padre Entity. 
	// Asumimos que la posición inicial (pos) ya se estableció antes de llamar a init.
	size = glm::vec2(16, 32);
	active = true;

	spritesheet.loadFromFile("images/enemies_sprite_sheet.png", TEXTURE_PIXEL_FORMAT_RGBA);

	float frameWidth = 1.0f / 22.0f;
	float frameHeight = 1.0f / 17.0f;

	sprite = Sprite::createSprite(size, glm::vec2(frameWidth, frameHeight), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	// --- NUEVO: Animación de muerte (Ajusta las coordenadas UV reales) ---
	sprite->setAnimationSpeed(DIE, 3); // Velocidad de la animación de muerte

	// Supongamos que en tu spritesheet, la fila 10 tiene los frames de muerte.
	// Y son los primeros 3 frames (col 0, 1, 2)
	float FILA_MUERTE = 3.0f;

	sprite->addKeyframe(DIE, glm::vec2(frameWidth * 12.0f, frameHeight * FILA_MUERTE));
	sprite->addKeyframe(DIE, glm::vec2(frameWidth * 13.0f, frameHeight * FILA_MUERTE));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(frameWidth * 3.0f, frameHeight * 2.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(frameWidth * 4.0f, frameHeight * 2.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(frameWidth * 5.0f, frameHeight * 2.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(frameWidth * 4.0f, frameHeight * 2.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(frameWidth * 0.0f, frameHeight * 2.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(frameWidth * 1.0f, frameHeight * 2.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(frameWidth * 2.0f, frameHeight * 2.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(frameWidth * 1.0f, frameHeight * 2.0f));

	sprite->changeAnimation(MOVE_RIGHT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

// EL NUEVO UPDATE CON FORMA DE ENTITY
int Piolin::update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) 
{
	if (!active) return 0; // Si está muerto/inactivo, no hace nada
	if (player->isEnemiesFrozen()) { // Si el jugador tiene el efecto de congelar enemigos, Piolín no se mueve ni actualiza su animación de movimiento
		return 0;
	}

	// --- NUEVO: Si está muriendo, detenemos la patrulla ---
	if (isDying) {
		deathTimer += deltaTime;
		if (sprite) sprite->update(deltaTime); // Seguimos actualizando el frame de la animación

		// Si han pasado 1.5 segundos (1500ms), lo hacemos desaparecer de verdad
		if (deathTimer > 1500) {
			active = false; // ¡Desaparecido de la Escena!
		}

		// Esencial: Mantener la posición sprite fija pero NO mover posPiolin
		if (sprite) sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
		return 0; // Cortamos el update para que no ejecute la lógica de patrulla
	}

	sprite->update(deltaTime);
	int dummyY;

	// Lógica de patrulla de lado a lado
	if (movingRight)
	{
		pos.x += speed;
		if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);

		bool chocaPared = map->collisionMoveRight(glm::ivec2(pos.x, pos.y + 16), glm::ivec2(16, 16));
		bool finMapa = pos.x >= (map->getMapSize().x * map->getTileSize()) - 16;
		bool haySuelo = map->collisionMoveDownPiolin(glm::ivec2(pos.x + 15, pos.y + 2), glm::ivec2(1, 32), &dummyY);

		// --- NUEVO: ¿Chocamos con una pesa inerte? ---
		bool chocaPesa = false;
		for (size_t i = 0; i < entities.size(); i++) {
			if (entities[i]->getType() == ENTITY_WEIGHT && entities[i]->isActive()) {
				if (checkAABBCollision(pos, size, entities[i]->getPos(), entities[i]->getSize())) {
					chocaPesa = true;
					break;
				}
			}
		}

		// Añadimos chocaPesa a la condición para darse la vuelta
		if (chocaPared || finMapa || !haySuelo || chocaPesa)
		{
			pos.x -= speed;
			movingRight = false;
		}
	}
	else
	{
		pos.x -= speed;
		if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);

		bool chocaPared = map->collisionMoveLeft(glm::ivec2(pos.x, pos.y + 16), glm::ivec2(16, 16));
		bool inicioMapa = pos.x <= 0;
		bool haySuelo = map->collisionMoveDownPiolin(glm::ivec2(pos.x, pos.y + 2), glm::ivec2(1, 32), &dummyY);

		// --- NUEVO: ¿Chocamos con una pesa inerte? ---
		bool chocaPesa = false;
		for (size_t i = 0; i < entities.size(); i++) {
			if (entities[i]->getType() == ENTITY_WEIGHT && entities[i]->isActive()) {
				if (checkAABBCollision(pos, size, entities[i]->getPos(), entities[i]->getSize())) {
					chocaPesa = true;
					break;
				}
			}
		}

		// Añadimos chocaPesa a la condición para darse la vuelta
		if (chocaPared || inicioMapa || !haySuelo || chocaPesa)
		{
			pos.x += speed;
			movingRight = true;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));

	// 2. COLISIÓN CON EL JUGADOR (MATA A BUGS BUNNY)
	// Como Piolín sabe dónde está el jugador (se lo pasamos por parámetro), comprobamos aquí.
	glm::vec2 pPos = player->getPosition();
	glm::vec2 pSize = glm::vec2(16, 32);

	if (checkAABBCollision(pPos, pSize, pos, size)) {
		player->die(numvidasmenos);
	}

	return 0; // Piolín no teletransporta, devuelve 0
}

// --- 3. IMPLEMENTACIÓN DE DIE ---
void Piolin::die() {
	if (isDying) return; // Si ya está muriendo, no hacemos nada

	isDying = true;
	deathTimer = 0;
	if (sprite) sprite->changeAnimation(DIE); // ¡Música, maestro! (bueno, animación)
}