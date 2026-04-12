#ifndef _PERRO_INCLUDE
#define _PERRO_INCLUDE

#include "Entity.h"
#include "TileMap.h"

//La clase Perro hereda de Entity y funciona igual que Piolin
class Perro : public Entity
{
public:
	Perro();

	void init(const glm::vec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	int update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) override;
	void die() override;
	// Añadir debajo de void die() override;
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
	int numvidasmenos = 2;//Si le chocamos nos quita 2 vidas en lugar de 1, para hacerlo mas dificil
};

#endif // _PERRO_INCLUDE