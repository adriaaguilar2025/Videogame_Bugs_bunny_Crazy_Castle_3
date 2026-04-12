#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Piolin.h"
#include <map>
#include "Entity.h"
#include "Key.h"
#include "Door.h"
#include "FinalDoor.h"
#include "Weight.h"
#include "Clock.h"
#include "Bomb.h"
#include "Texture.h"
#include "Perro.h"
#include "Pato.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void loadLevel(int levelNumber); //Nuevo método para cargar un nivel concreto
	void clearObjects();

private:
	void initShaders();
	enum SceneState {
		MENU,
		INSTRUCTIONS,
		LEVEL_MENU,
		PLAYING,
		CREDITS // <--- ¡Añadido!
	};
	SceneState currentState; // Guardará el estado actual

	// --- VARIABLES DEL MENU ---
	Texture menuTexture;     // La textura del fondo
	Sprite* menuSprite;      // El sprite para dibujar el fondo
	//________________________________
	Texture instructionsTexture; // ¡NUEVO!
	Sprite* instructionsSprite;  // ¡NUEVO!

	Texture levelMenuTexture;    // ¡NUEVO!
	Sprite* levelMenuSprite;     // ¡NUEVO!
	Texture creditsTexture;
	Sprite* creditsSprite;
	bool enterWasPressed = false;

	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int currentLevel; //variable para saber el nivel actual
	Texture itemsSpritesheet;
	glm::vec2 savedPlayerPos;
	bool isReturningFromSubroom = false;
	float teleportCooldown;
	// Memoria de teclas para evitar el spam de consola
	bool gKeyWasPressed = false;
	bool kKeyWasPressed = false;
	bool key1WasPressed = false;
	bool key2WasPressed = false;
	bool key3WasPressed = false;
	bool key4WasPressed = false;
	bool key5WasPressed = false;
	bool rKeyWasPressed = false;
	bool bKeyWasPressed = false;

	void spawnBomb(const glm::vec2& pos); //Función para crear la bomba nueva encendida en la posición del jugador
	void spawnSpeedBoost(const glm::vec2& pos);//Función para crear el objeto de velocidad en la posición del jugador

	std::vector<Entity*> entities; // ¡La magia del polimorfismo! Sustituye a keys y doors
	// Sustituye el map de bools por este bloque:
	struct EntityMemory {
		bool active;
		int state;
		float posX;
		float posY; // ¡NUEVO! Necesario para el pato que sube y baja
		int direction; // Guardamos hacia dónde miraban
	};
	std::map<int, std::vector<EntityMemory>> entitiesMemory;
	bool bLoading; // Variable para evitar que se actualice la lógica de objetos mientras se está cargando un nivel nuevo
	std::map<int, std::vector<float>> entitiesPosXMemory;
	// --- VARIABLES DEL HUD ---
	Texture hudBugsSheet;
	Texture hudItemsSheet;
	Texture hudNumbersSheet; //Textura para los números
	Sprite* hudIconFace;
	Sprite* hudIconKey;
	Sprite* hudNumber;       //Vuelve el sprite de los números
	Sprite* hudIconClock; //VARIABLE PARA EL ICONO DEL RELOJ
	Sprite* hudIconBomb; //Variable para el icono de la bomba
	int parentLevel;

};


#endif // _SCENE_INCLUDE

