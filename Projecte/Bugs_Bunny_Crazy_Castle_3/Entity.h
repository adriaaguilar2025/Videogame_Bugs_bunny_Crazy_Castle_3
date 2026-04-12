#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include "Sprite.h"
#include "Player.h"
#include <glm/glm.hpp>
#include <vector>

// Nos sirve para saber qué objeto es sin complicarnos la vida
enum EntityType { ENTITY_KEY, ENTITY_DOOR, ENTITY_FINAL_DOOR, ENTITY_ENEMY, ENTITY_WEIGHT, ENTITY_CLOCK, ENTITY_BOMB, ENTITY_SPEED_BOOST, ENTITY_PERRO};

class Entity {
public:
	Entity() : sprite(NULL), active(true), actionValue(0), state(0), type(ENTITY_KEY) {}
	virtual ~Entity() { if (sprite != NULL) delete sprite; }

	// ¡ATENCIÓN! Hemos añadido el vector de entidades al update para que FinalDoor pueda buscar las llaves
	virtual int update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) = 0;

	virtual void render() { if (active && sprite) sprite->render(); }

	bool isActive() const { return active; }
	void setActive(bool b) { active = b; }

	glm::vec2 getPos() const { return pos; }
	void setPos(const glm::vec2& p) { pos = p; }
	glm::vec2 getSize() const { return size; }

	// Nuevos métodos para el Estado (abierta/cerrada)
	int getState() const { return state; }
	virtual void setState(int s) { state = s; }

	EntityType getType() const { return type; }
	virtual void die() { active = false; }
	// --- NUEVOS MÉTODOS PARA MEMORIA DE ENEMIGOS ---
	virtual bool isEntityDying() const { return false; } // Por defecto no mueren con timer
	virtual int getDirection() const { return 0; } // 0 = Izquierda, 1 = Derecha
	virtual void setDirection(int d) {}

protected:
	Sprite* sprite;
	glm::vec2 pos;
	glm::vec2 size;
	bool active;
	int actionValue;
	int state;
	EntityType type;

	bool checkAABBCollision(const glm::vec2& pos1, const glm::vec2& size1, const glm::vec2& pos2, const glm::vec2& size2) {
		return (pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x &&
			pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y);
	}
};

#endif // _ENTITY_INCLUDE