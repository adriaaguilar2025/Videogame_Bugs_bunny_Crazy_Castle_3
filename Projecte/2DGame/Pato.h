#ifndef _PATO_INCLUDE
#define _PATO_INCLUDE

#include "Entity.h"
#include "TileMap.h"

class Pato : public Entity
{
public:
	Pato();

	void init(const glm::vec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	int update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) override;
	void die() override;

	// Memoria de estado
	bool isEntityDying() const override { return isDying; }
	int getDirection() const override { return movingRight ? 1 : 0; }
	void setDirection(int d) override { movingRight = (d == 1); }

private:
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	TileMap* map;

	bool movingRight;
	float speed;
	bool isDying;
	int deathTimer;
	int numvidasmenos = 1; // Quita 1 vida, como Piolín
	bool isClimbing;       // ¡NUEVO! Para la animación de trepar
};

#endif // _PATO_INCLUDE