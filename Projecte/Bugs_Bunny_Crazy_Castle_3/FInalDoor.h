#ifndef _FINALDOOR_INCLUDE
#define _FINALDOOR_INCLUDE
#include "Door.h"

class FinalDoor : public Door {
public:
	// Añadimos 'reqKeys' al final del init
	void init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp, int dest, int reqKeys);
	int update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) override;
private:
	int requiredKeys; // Guardará cuántas llaves exige esta puerta
};
#endif