#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{
public:
	Player();
	~Player();

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition() const { return posPlayer; }

	void addKey() { keysCollected++; }
	void useKey() { if (keysCollected > 0) keysCollected--; }
	int getKeys() const { return keysCollected; }
	void setKeys(int k) { keysCollected = k; }

	void checkCollisionWithEnemy(const glm::vec2& enemyPos, const glm::ivec2& enemySize);//Comprueba si el jugador colisiona con un enemigo, y si es así, lo mata
	bool isReadyToRespawn() const { return readyToRespawn; }
	void die(int numvidasmenos);
	void reviu();
	int getLives() const { return lives; }
	void setLives(int l) { lives = l; }
	void loseLife() { if (lives > 0) lives--; }
	bool isGodMode() const { return godMode; }
	void toggleGodMode() { godMode = !godMode; }

	//Metodos relacionados con los relojes
	void addClock() { clocksCollected++; }
	void useClock();
	int getClocks() const { return clocksCollected; }
	void setClocks(int c) { clocksCollected = c; }
	bool isEnemiesFrozen() const { return freezeTimer > 0; } //Permite saber a los enemigos si están congelados

	//Metodos relacionados con las bombas
	void addBomb() { bombs++; }
	void useBomb() { if (bombs > 0) bombs--; }
	int getBombs() const { return bombs; }
	void setBombs(int b) { bombs = b; }

	//Objeto de velocidad
	void activateSpeedBoost();
	void stopSpeedBoost();
	
private:
	bool bJumping;//Estado para saber si el jugador está saltando o no, para aplicar la gravedad o no.
	bool bfalling;//Estado para saber si el jugador está cayendo o no, para aplicar animaciones.
	float speed = 2.0f;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int keysCollected = 0;
	bool isDead;//Estado para saber si el jugador ha muerto, para iniciar el timer de respawn
	int deathTimer;
	bool readyToRespawn;
	int lives;
	bool godMode;

	//Variables relacionadas con los relojes
	int clocksCollected;
	int freezeTimer; // Temporizador en milisegundos

	int bombs = 0;//Cantidad de bombas que tiene el jugador
	int speedBoostTimer; //Temporizador para la velocidad
};


#endif // _PLAYER_INCLUDE


