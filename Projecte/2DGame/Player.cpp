#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 3
#define JUMP_HEIGHT 96 //6 tiles de altura maxima al hacer el salto, cada tile mide 16 píxeles, así que 6*16=96
#define FALL_STEP 4

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP, FALL, DIE, CLIMB // <-- ¡Añadimos CLIMB aquí!
};


Player::Player()
{
	sprite = NULL;
	map = NULL;
}

Player::~Player()
{
	if (sprite != NULL)
		delete sprite;
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	bfalling = false;
	keysCollected = 0;
	isDead = false;
	deathTimer = 0;
	readyToRespawn = false;
	lives = 3;
	godMode = false;
	clocksCollected = 0;
	freezeTimer = 0;
	bombs = 0;
	speedBoostTimer = 0;

	spritesheet.loadFromFile("images/bugs_bunny_sprite_sheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//Primero, es la medida que queremos que tenga el sprite en pantalla (en píxeles), y luego, la medida que ocupa cada frame dentro del spritesheet (en coordenadas de textura, de 0 a 1).
	float frameWidth = 1.0f / 17.0f; //El spritesheet tiene 17 columnas
	float frameHeight = 1.0f / 7.0f; //El spritesheet tiene 7 filas
	//Lo dejo sin implementar, ya que el tilesheet del bugs bunny esta muy mal hecho y no se pueden coger frames sueltos sin coger partes de otros frames. Para el siguiente ejercicio, usaremos un spritesheet mejor hecho, así que lo dejo sin implementar para no perder tiempo con esto.

		sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(frameWidth, frameHeight), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(8);

		sprite->setAnimationSpeed(STAND_LEFT, 8);
		//Has de poner de parametros primero el numero de columna (0-16) y luego el numero de fila (0-6), multiplicados por el frameWidth y frameHeight respectivamente, para obtener las coordenadas de textura correctas.
		sprite->addKeyframe(STAND_LEFT, glm::vec2(frameWidth * 4.0f , frameHeight * 0.0f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(frameWidth * 5.0f, frameHeight * 0.0f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(frameWidth * 0.0f, frameHeight * 0.0f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(frameWidth * 1.0f, frameHeight * 0.0f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(frameWidth * 2.0f, frameHeight * 0.0f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(frameWidth * 3.0f, frameHeight * 0.0f));

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(frameWidth * 6.0f, frameHeight * 0.0f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(frameWidth * 7.0f, frameHeight * 0.0f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(frameWidth * 8.0f, frameHeight * 0.0f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(frameWidth * 9.0f, frameHeight * 0.0f));

		sprite->setAnimationSpeed(JUMP, 8);
		sprite->addKeyframe(JUMP, glm::vec2(frameWidth * 10.0f, frameHeight * 0.0f));
		sprite->addKeyframe(JUMP, glm::vec2(frameWidth * 11.0f, frameHeight * 0.0f));

		sprite->setAnimationSpeed(FALL, 8);
		sprite->addKeyframe(FALL, glm::vec2(frameWidth * 12.0f, frameHeight * 0.0f));
		sprite->addKeyframe(FALL, glm::vec2(frameWidth * 13.0f, frameHeight * 0.0f));

		sprite->setAnimationSpeed(DIE, 2);
		sprite->addKeyframe(DIE, glm::vec2(frameWidth * 4.0f, frameHeight * 1.0f));
		sprite->addKeyframe(DIE, glm::vec2(frameWidth * 5.0f, frameHeight * 1.0f));
		sprite->addKeyframe(DIE, glm::vec2(frameWidth * 6.0f, frameHeight * 1.0f));
		sprite->addKeyframe(DIE, glm::vec2(frameWidth * 7.0f, frameHeight * 1.0f));

		sprite->setAnimationSpeed(CLIMB, 8);
		float FILA_TREPAR = 2.0f;
		sprite->addKeyframe(CLIMB, glm::vec2(frameWidth * 0.0f, frameHeight * FILA_TREPAR));
		sprite->addKeyframe(CLIMB, glm::vec2(frameWidth * 1.0f, frameHeight * FILA_TREPAR));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	int currentSpeed = speed;

	//LÓGICA DEL BOOST DE VELOCIDAD
	if (speedBoostTimer > 0) {
		speedBoostTimer -= deltaTime;
		if (speedBoostTimer <= 0) {
			speed = 2.0f; //Se acabó el tiempo, volvemos a la velocidad normal
			speedBoostTimer = 0;
		}
	}

	//Comprobamos si estamos sobre un bloque pegajoso
	// Usamos el tamaño del jugador: ancho 16, alto 32
	if (map->isSticky(glm::ivec2(posPlayer.x, posPlayer.y), glm::ivec2(16, 32)))
	{
		currentSpeed = currentSpeed / 2; // ¡Reducimos la velocidad a la mitad!
	}

	//restamos el tiempo al temporizador si está activo :
	if (freezeTimer > 0) {
		freezeTimer -= deltaTime;
		if (freezeTimer < 0) freezeTimer = 0;
	}
	
	//Miramos si el jugador ha muerto, y si es así solo actualizamos el timer de muerte y hacemos un return para que no ejecute el código de movimiento ni salto.
	if (isDead) {
		deathTimer += deltaTime;
		// Si han pasado 2000 milisegundos (2 segundos), activamos el respawn
		if (deathTimer >= 2000) {
			readyToRespawn = true;
		}
		return;
	}

	//MOVIMIENTO HORIZONTAL
	if (Game::instance().getKey(GLFW_KEY_LEFT))
	{
		if (!bJumping && !bfalling && sprite->animation() != MOVE_LEFT)//Si Estamos saltando o cayendo no cambiamos la animacion
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= currentSpeed;
		// Le sumamos 16 a la Y para que solo choque la mitad inferior
		if (map->collisionMoveLeft(glm::ivec2(posPlayer.x, posPlayer.y + 16), glm::ivec2(16, 16)))
		{
			posPlayer.x += currentSpeed;
			sprite->changeAnimation(STAND_LEFT);
		}
		//EL MURO INVISIBLE: Sila posición X baja de 0, lo clavamos en 0.
		if (posPlayer.x < 0)
		{
			posPlayer.x = 0;
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if (!bJumping && !bfalling && sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);

		posPlayer.x += currentSpeed;
		if (map->collisionMoveRight(glm::ivec2(posPlayer.x, posPlayer.y + 16), glm::ivec2(16, 16)))
		{
			posPlayer.x -= currentSpeed;
			sprite->changeAnimation(STAND_RIGHT);
		}

		// ¡EL FIX DEL MURO INVISIBLE DINÁMICO!
		// Calculamos el ancho del mapa en píxeles y le restamos el ancho de Bugs (16)
		int limiteDerecho = (map->getMapSize().x * map->getTileSize()) - 16;

		if (posPlayer.x > limiteDerecho)
		{
			posPlayer.x = limiteDerecho;
		}
	}

	// Si no pulsamos ni izquierda ni derecha, nos quedamos quietos
	else
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	// 2. MOVIMIENTO VERTICAL (Escaleras / Enredaderas)
	if (Game::instance().getKey(GLFW_KEY_UP))
	{
		if (map->isOnStairs(glm::ivec2(posPlayer.x, posPlayer.y + 16), glm::ivec2(16, 16)))
		{
			// Subimos normal, ¡sin efecto imán!
			if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
			posPlayer.y -= 2;

			// Si al subir nos salimos del imán de la liana, deshacemos el paso
			if (!map->isOnStairs(glm::ivec2(posPlayer.x, posPlayer.y + 16), glm::ivec2(16, 16)))
			{
				posPlayer.y += 2;
			}

			bJumping = false;
			bfalling = false;
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_DOWN))
	{
		if (map->isOnStairs(glm::ivec2(posPlayer.x, posPlayer.y + 16), glm::ivec2(16, 16)))
		{
			if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
			// ¡EL FIX INTELIGENTE!
			// Como Bugs y el agujero miden lo mismo (16px), si sobresale un solo píxel,
			// chocará con la roca y no bajará. Lo alineamos automáticamente.

			int tileLeft = posPlayer.x / 16;
			int tileRight = (posPlayer.x + 15) / 16;

			// Lanzamos un "láser" a los pies de Bugs para ver de qué lado asoma la enredadera
			bool pieIzquierdo = map->isOnStairs(glm::ivec2(posPlayer.x, posPlayer.y + 16), glm::ivec2(1, 16));
			bool pieDerecho = map->isOnStairs(glm::ivec2(posPlayer.x + 15, posPlayer.y + 16), glm::ivec2(1, 16));

			// Si estamos un poco torcidos, nos centramos en la columna correcta
			if (pieIzquierdo && !pieDerecho) {
				posPlayer.x = tileLeft * 16;   // Alinearse a la casilla izquierda
			}
			else if (pieDerecho && !pieIzquierdo) {
				posPlayer.x = tileRight * 16;  // Alinearse a la casilla derecha
			}

			// Ahora que estamos perfectamente encajados en el tubo, bajamos libres
			posPlayer.y += 2;
			map->collisionMoveDown(posPlayer, glm::ivec2(16, 32), &posPlayer.y);

			bJumping = false;
			bfalling = false;
		}
	}
	// 1. SI ESTAMOS SALTANDO (Aquí es donde se actualiza la 'Y')
	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;

		if (jumpAngle == 180) //Fin de la parábola del salto
		{
			bJumping = false;
			bfalling = true; //Empezamos a caer
			posPlayer.y = startY;
		}
		else
		{
			//Calculamos la nueva posición Y usando el seno ,esto es lo que mueve al jugador hacia arriba suavemente
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));

			//Si ya estamos cayendo (pasamos del punto más alto)
			if (jumpAngle > 90) {
				bJumping = false;
				bfalling = true; //Empezamos a caer
			}
		}
	}

	//3. GRAVEDAD (Caer por los bordes)
	//Solo aplicamos gravedad si NO estamos agarrados a una escalera o si no estamos saltando
	if (!bJumping && !map->isOnStairs(glm::ivec2(posPlayer.x, posPlayer.y + 16), glm::ivec2(16, 16)))
	{
		posPlayer.y += FALL_STEP;
		
		if (map->collisionMoveDown(posPlayer, glm::ivec2(16, 32), &posPlayer.y)){
			//Si hay colision, entonces estamos en el suelo
			bfalling = false;//No estamos cayendo, estammos en el suelo, cambiamos la animacion.
			if (sprite->animation() == FALL)
			{
				sprite->changeAnimation(STAND_LEFT);
			}
			//miramos si el suelo es el tile 51 (El up)
			if (map->isTrampoline(posPlayer, glm::ivec2(16, 32)))
			{
				bJumping = true;      //Activamos el salto
				jumpAngle = 0;        //Reiniciamos el ángulo
				startY = posPlayer.y; // Guardamos desde dónde salta
			}
		}
		else {//NO hay colision, por lo que estamos en el aire
			bfalling = true; //Si no estamos saltando, pero estamos en el aire, entonces estamos cayendo
		}
	}

	///////////////////Animaciones de salto y caida/////////////////////
	if (bJumping) {
		if (sprite->animation() != JUMP)
			sprite->changeAnimation(JUMP);
	}
	else if(bfalling) {
		if (sprite->animation() != FALL)
			sprite->changeAnimation(FALL);
	}



	// Actualizamos la posición real del sprite en pantalla
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::checkCollisionWithEnemy(const glm::vec2& enemyPos, const glm::ivec2& enemySize)
{
	if (isDead) return; //Si ya está muerto, no comprobamos nada

	glm::ivec2 playerSize(16, 32);

	//Detección
	bool collisionX = posPlayer.x < enemyPos.x + enemySize.x && posPlayer.x + playerSize.x > enemyPos.x;
	bool collisionY = posPlayer.y < enemyPos.y + enemySize.y && posPlayer.y + playerSize.y > enemyPos.y;

	if (collisionX && collisionY) {
		die(1);
	}
}

void Player::die(int numvidasmenos)
{
	if (godMode || isDead) return;
	isDead = true;
	deathTimer = 0;
	for (int i = 0; i < numvidasmenos; i++) {//Le quitamos el numero de vidas que nos ha dicho el enemigo (lose life controla los caasos negativos)
		loseLife();
	}
	sprite->changeAnimation(DIE);
}
void Player::reviu()
{
	isDead = false;
	readyToRespawn = false;
	sprite->changeAnimation(STAND_LEFT);//Le quitamo la animacion de muerte
}

void Player::useClock() {
	if (clocksCollected > 0 && freezeTimer <= 0) { // Solo si tienes relojes y no hay un efecto ya activo
		clocksCollected--;
		freezeTimer = 5000; // 5000 milisegundos = 5 segundos
		std::cout << "Enemigos congelados por 5 seg. Relojes restantes: " << clocksCollected << std::endl;
	}
}

void Player::activateSpeedBoost() {
	speed = 3.0f;           //multiplicamos la velocidad por 1.5
	speedBoostTimer = 10000; //10 segundos en milisegundos
}

void Player::stopSpeedBoost() {
	speed = 2.0f;           //Volvemos a la velocidad normal
	speedBoostTimer = 0.0f; //Cancelamos el temporizador
}