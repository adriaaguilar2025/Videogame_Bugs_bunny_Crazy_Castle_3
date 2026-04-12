#ifndef _WEIGHT_INCLUDE
#define _WEIGHT_INCLUDE
#include "Entity.h"
#include "TileMap.h"

class Weight : public Entity {
public:
	// Le pasamos el mapa para que sepa dónde chocar y el desplazamiento de pantalla para dibujarse
	void init(const glm::vec2& position, const glm::vec2& sz, Sprite* sp, TileMap* tm, const glm::vec2& displ);

	int update(int deltaTime, Player* player, float teleportCooldown, const std::vector<Entity*>& entities) override;

private:
	TileMap* map;
	glm::vec2 tileMapDispl;
};
#endif