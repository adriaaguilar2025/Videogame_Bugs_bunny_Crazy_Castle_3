#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE
#include "Entity.h"

class Door : public Entity {
public:
	virtual void init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp, int dest);
	int update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) override;
	void setState(int s) override;
};
#endif