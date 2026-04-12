#ifndef _PIOLIN_INCLUDE
#define _PIOLIN_INCLUDE

#include "Entity.h" // ¡Añadimos esto!
#include "TileMap.h"

// Piolin AHORA HEREDA DE Entity
class Piolin : public Entity
{
public:
	Piolin();
	// El destructor virtual ya está en Entity, y sprite se borra ahí.

	// Cambiamos init para que encaje mejor con el sistema
	void init(const glm::vec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);

	// IMPLEMENTAMOS EL UPDATE OBLIGATORIO DE ENTITY
	int update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) override;

	// La función render() ya viene de Entity, la usamos tal cual
	void die() override;
	// Añadir debajo de void die() override;
	bool isEntityDying() const override { return isDying; }
	int getDirection() const override { return movingRight ? 1 : 0; }
	void setDirection(int d) override { movingRight = (d == 1); }

private:
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	TileMap* map;

	//Variables exclusivas de Piolín
	bool movingRight;
	int speed;
	bool isDying;
	int deathTimer;
	int numvidasmenos = 1; //Si le chocamos nos quita 1 vida
};

#endif // _PIOLIN_INCLUDE