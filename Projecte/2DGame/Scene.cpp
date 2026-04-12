
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") // Esto vincula la librería de audio de Windows automáticamente
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <algorithm> // Para poder usar std::max y std::min
#include <fstream>
#include "Key.h"
#include "Door.h"
#include "Weight.h"
#include "SpeedBoost.h"

 
#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 1
#define INIT_PLAYER_Y_TILES 23

#define INIT_PLAYER_X_TILES_SUBLEVEL 4
#define INIT_PLAYER_Y_TILES_SUBLEVEL 7

Scene::Scene()
{
	map = NULL;
	player = NULL;
	teleportCooldown = 0.0f;
	parentLevel = 1;

	//Inicializamos los punteros del HUD a NULL por seguridad
	hudIconFace = NULL;
	hudIconKey = NULL;
	hudIconClock = NULL;
	hudIconBomb = NULL;
}

Scene::~Scene()
{
	texProgram.free();
	if (map != NULL) delete map;
	if (player != NULL) delete player;

	// Limpieza del HUD (Actualizado con los nombres reales)
	if (hudIconFace != NULL) delete hudIconFace;
	if (hudIconKey != NULL) delete hudIconKey;
	if (hudNumber != NULL) delete hudNumber;
	if (hudIconClock != NULL) delete hudIconClock;
	if (hudIconBomb != NULL) delete hudIconBomb;
}

void Scene::init()
{
	initShaders();
	// Asegúrate de tener una carpeta "audio" con tu archivo .wav dentro
	PlaySound(TEXT("audio/musica_fondo.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//Init menu:
	currentState = MENU;
	menuTexture.loadFromFile("images/MenuInicio.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menuSprite = Sprite::createSprite(glm::vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f), glm::vec2(1.0, 1.0), &menuTexture, &texProgram);
	menuSprite->setPosition(glm::vec2(0, 0));

	// --- NUEVAS PANTALLAS ---
	instructionsTexture.loadFromFile("images/instruccions.png", TEXTURE_PIXEL_FORMAT_RGB);
	instructionsSprite = Sprite::createSprite(glm::vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f), glm::vec2(1.0, 1.0), &instructionsTexture, &texProgram);
	instructionsSprite->setPosition(glm::vec2(0, 0));

	levelMenuTexture.loadFromFile("images/menuNiveles.png", TEXTURE_PIXEL_FORMAT_RGB);
	levelMenuSprite = Sprite::createSprite(glm::vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f), glm::vec2(1.0, 1.0), &levelMenuTexture, &texProgram);
	levelMenuSprite->setPosition(glm::vec2(0, 0));

	creditsTexture.loadFromFile("images/creditos.png", TEXTURE_PIXEL_FORMAT_RGBA);
	creditsSprite = Sprite::createSprite(glm::vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f), glm::vec2(1.0, 1.0), &creditsTexture, &texProgram);
	creditsSprite->setPosition(glm::vec2(0, 0));

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	// --- CARGA DEL HUD RECICLADO (Scene::init) ---
	// Cargamos las imágenes existentes directamente en el init
	hudBugsSheet.loadFromFile("images/bugs_bunny_sprite_sheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	hudItemsSheet.loadFromFile("images/objetos_sprite_sheet.png", TEXTURE_PIXEL_FORMAT_RGBA); // Asignamos la textura de objetos
	// 1. Icono de Vidas (Buscando la cara de frente)
	float bugsWidth = 1.0f / 17.0f; // 17 columnas
	float bugsHeight = 1.0f / 7.0f; // 7 filas

	// CAMBIA ESTOS DOS NÚMEROS: 
	// Prueba la columna 2, 3 o 4. Y la fila 0, 1 o 2 hasta que salga de frente.
	float columnaCara = 4.0f;
	float filaCara = 1.0f;

	// Nota: Bugs Bunny mide 16x32. Si el icono de 16x16 se ve "aplastado", 
	// cambia el primer vec2 a (16, 32)
	hudIconFace = Sprite::createSprite(glm::vec2(16, 16), glm::vec2(bugsWidth, bugsHeight), &hudBugsSheet, &texProgram);
	hudIconFace->setNumberAnimations(1);
	hudIconFace->addKeyframe(0, glm::vec2(bugsWidth * columnaCara, bugsHeight * filaCara));
	hudIconFace->changeAnimation(0);

	// 2. Icono de la Llave (¡Usando las matemáticas exactas de tu .txt!)
	// Imagen de 255x168. Llave de 16x16. Píxel inicial X=0, Y=9.
	float keyUvWidth = 15.0f / 255.0f;
	float keyUvHeight = 15.0f / 168.0f;
	float keyUvX = 0.0f / 255.0f;
	float keyUvY = 9.0f / 168.0f;

	hudIconKey = Sprite::createSprite(glm::vec2(16, 16), glm::vec2(keyUvWidth, keyUvHeight), &hudItemsSheet, &texProgram);
	hudIconKey->setNumberAnimations(1);
	hudIconKey->addKeyframe(0, glm::vec2(keyUvX, keyUvY));
	hudIconKey->changeAnimation(0);

	// 3. El Sprite de los Números (PNG de 160x16 -> 10 columnas, 1 fila)
	hudNumbersSheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	// Como solo hay 1 fila, la altura en el spritesheet es 1.0f (el 100%)
	hudNumber = Sprite::createSprite(glm::vec2(16, 16), glm::vec2(1.0f / 10.0f, 1.0f), &hudNumbersSheet, &texProgram);
	hudNumber->setNumberAnimations(10); // 10 números (0 al 9)

	// 4. ICONO DEL RELOJ
	// ----------------------------------------------------
	float clockPixelX = 120.0f; 
	float clockPixelY = 9.0f;

	float clockUvX = clockPixelX / 255.0f;
	float clockUvY = clockPixelY / 168.0f;
	float clockUvWidth = 15.0f / 255.0f;
	float clockUvHeight = 15.0f / 168.0f;

	hudIconClock = Sprite::createSprite(glm::vec2(16, 16), glm::vec2(clockUvWidth, clockUvHeight), &hudItemsSheet, &texProgram);
	hudIconClock->setNumberAnimations(1);
	hudIconClock->addKeyframe(0, glm::vec2(clockUvX, clockUvY));
	hudIconClock->changeAnimation(0);

	//5. Icono de la Bomba
	float frameWidth = 1.0f / 17.0f;
	float frameHeight = 1.0f / 7.0f;

	hudIconBomb = Sprite::createSprite(glm::vec2(16, 16), glm::vec2(frameWidth, frameHeight), &itemsSpritesheet, &texProgram);
	hudIconBomb->setNumberAnimations(1);
	hudIconBomb->setAnimationSpeed(0, 8);
	hudIconBomb->addKeyframe(0, glm::vec2(frameWidth * 2, frameHeight * 0)); //Sprite de la bomba apagada
	hudIconBomb->changeAnimation(0);

	for (int i = 0; i < 10; i++) {
		hudNumber->setAnimationSpeed(i, 1);
		// X avanza según el número (0.0, 0.1, 0.2...), Y es siempre 0.0f porque solo hay una fila
		hudNumber->addKeyframe(i, glm::vec2(float(i) / 10.0f, 0.0f));
	}
	hudNumber->changeAnimation(0);

	loadLevel(1);

	// EL TRUCO DEL ZOOM: Dividimos la pantalla entre el zoom
	float zoom = 2.0f; // Puedes cambiarlo a 2.5f o 3.0f si lo quieres MÁS grande
	projection = glm::ortho(0.f, float(SCREEN_WIDTH) / zoom, float(SCREEN_HEIGHT) / zoom, 0.f);
	currentTime = 0.0f;

}

void Scene::update(int deltaTime)
{
	// 1. Lógica del botón Enter (Solo se activa en el frame exacto que lo pulsas)
	bool currentEnter = Game::instance().getKey(GLFW_KEY_ENTER);
	bool enterPressed = currentEnter && !enterWasPressed;
	enterWasPressed = currentEnter; // Guardamos el estado para el siguiente frame

	// 2. Máquina de Estados de los Menús
	if (currentState == MENU) {
		if (enterPressed) {
			currentState = INSTRUCTIONS; // Pasamos a instrucciones
		}
		return; // Cortamos el update para que no pase el tiempo ni se muevan cosas
	}
	else if (currentState == INSTRUCTIONS) {
		if (enterPressed) {
			currentState = LEVEL_MENU; // Pasamos al selector de niveles
		}
		return;
	}
	else if (currentState == LEVEL_MENU) {
		// Aquí movimos la selección de niveles
		if (Game::instance().getKey(GLFW_KEY_1)) { loadLevel(1); currentState = PLAYING; }
		else if (Game::instance().getKey(GLFW_KEY_2)) { loadLevel(2); currentState = PLAYING; }
		else if (Game::instance().getKey(GLFW_KEY_3)) { loadLevel(3); currentState = PLAYING; }
		else if (Game::instance().getKey(GLFW_KEY_4)) { loadLevel(4); currentState = PLAYING; }
		else if (Game::instance().getKey(GLFW_KEY_5)) { loadLevel(5); currentState = PLAYING; }
		return;
	}
	else if (currentState == CREDITS) {
		if (enterPressed) {
			currentState = MENU; // Volver al inicio del todo
			player->setLives(3);
			player->setKeys(0);
		}
		return;
	}
	
	
	
	currentTime += deltaTime;
	if (player != NULL)
		player->update(deltaTime);//Actualizamos al jugador



	//Si el jugador ha muerto y ya pasaron los 2 segundos, recargamos el nivel
	// Si el jugador ha muerto y ya pasaron los 2 segundos, recargamos el nivel
	if (player != NULL && player->isReadyToRespawn()) {

		if (player->getLives() <= 0) {
			cout << "¡GAME OVER!" << endl;
			// Reseteo total del juego
			entitiesMemory.clear();
			entitiesPosXMemory.clear();
			currentLevel = 0;
			player->setKeys(0);
			player->setLives(3); // Le devolvemos las vidas
			player->reviu();
			currentState = MENU;
			loadLevel(1); // Volvemos al nivel 1
		}
		else {
			cout << "Vidas restantes: " << player->getLives() << endl;
			// Reseteo solo de la sala actual (Lo que ya tenías)
			entitiesMemory.erase(currentLevel);
			entitiesPosXMemory.erase(currentLevel);

			int nivelARecargar = currentLevel;
			if (currentLevel >= 100 && currentLevel < 200) nivelARecargar = 1;
			else if (currentLevel >= 200 && currentLevel < 300) nivelARecargar = 2;

			currentLevel = 0;
			player->setKeys(0);
			player->reviu();
			loadLevel(nivelARecargar);
		}
		return;
	}


	if (teleportCooldown > 0.0f) {
		teleportCooldown -= deltaTime;
	}

	// --- TECLAS DE DEBUG OBLIGATORIAS ---

	// 1. GOD MODE ('G')
	bool currentG = Game::instance().getKey('G');
	if (currentG && !gKeyWasPressed) {
		player->toggleGodMode();
		if (player->isGodMode()) cout << "\n[CHEAT] GOD MODE ACTIVADO: Eres inmortal." << endl;
		else cout << "\n[CHEAT] GOD MODE DESACTIVADO: Vuelves a ser mortal." << endl;
	}
	gKeyWasPressed = currentG;

	// 2. AÑADIR LLAVES ('K')
	bool currentK = Game::instance().getKey('K');
	if (currentK && !kKeyWasPressed) {
		player->setKeys(99);
		cout << "\n[CHEAT] 99 Llaves anadidas magicamente" << endl;
	}
	kKeyWasPressed = currentK;

	//USAR RELOJES ('R')
	if (Game::instance().getKey('r') || Game::instance().getKey('R')) {
		if (!rKeyWasPressed) {
			player->useClock(); // Intentamos usar el reloj
			rKeyWasPressed = true;
		}
	}
	else {
		rKeyWasPressed = false;
	}

	//LÓGICA DE SOLTAR BOMBA
	if (Game::instance().getKey('b') || Game::instance().getKey('B')) {
		if (!bKeyWasPressed && player->getBombs() > 0) {

			// Instanciamos una nueva bomba en la posición del jugador
			glm::vec2 playerPos = player->getPosition();
			spawnBomb(glm::vec2(playerPos.x, playerPos.y + 16.0f));// La bomba aparece justo encima de los pies del jugador

			// Restamos 1 al inventario
			player->useBomb();

		}
		bKeyWasPressed = true;
	}
	else {
		bKeyWasPressed = false;
	}

	// 3. CAMBIO DE NIVELES RÁPIDO (1 al 5)
	// Primero leemos si se acaban de pulsar en este frame exacto
	bool press1 = Game::instance().getKey(GLFW_KEY_1) && !key1WasPressed;
	bool press2 = Game::instance().getKey(GLFW_KEY_2) && !key2WasPressed;
	bool press3 = Game::instance().getKey(GLFW_KEY_3) && !key3WasPressed;
	bool press4 = Game::instance().getKey(GLFW_KEY_4) && !key4WasPressed;
	bool press5 = Game::instance().getKey(GLFW_KEY_5) && !key5WasPressed;

	// Inmediatamente guardamos el estado actual para el frame que viene
	key1WasPressed = Game::instance().getKey(GLFW_KEY_1);
	key2WasPressed = Game::instance().getKey(GLFW_KEY_2);
	key3WasPressed = Game::instance().getKey(GLFW_KEY_3);
	key4WasPressed = Game::instance().getKey(GLFW_KEY_4);
	key5WasPressed = Game::instance().getKey(GLFW_KEY_5);

	// Y ahora sí, si alguna fue pulsada, hacemos la acción y cortamos con el return
	if (press1) { cout << "\n[DEBUG] Reiniciando Nivel 1..." << endl; entitiesMemory.clear(); entitiesPosXMemory.clear(); player->setKeys(0); currentLevel = 0; loadLevel(1); return; }
	if (press2) { cout << "\n[DEBUG] Saltando al Nivel 2..." << endl; entitiesMemory.clear(); entitiesPosXMemory.clear(); player->setKeys(0); currentLevel = 0; loadLevel(2); return; }
	if (press3) { cout << "\n[DEBUG] Saltando al Nivel 3..." << endl; entitiesMemory.clear(); entitiesPosXMemory.clear(); player->setKeys(0); currentLevel = 0; loadLevel(3); return; }
	if (press4) { cout << "\n[DEBUG] Saltando al Nivel 4..." << endl; entitiesMemory.clear(); entitiesPosXMemory.clear(); player->setKeys(0); currentLevel = 0; loadLevel(4); return; }
	if (press5) { cout << "\n[DEBUG] Saltando al Nivel 5..." << endl; entitiesMemory.clear(); entitiesPosXMemory.clear(); player->setKeys(0); currentLevel = 0; loadLevel(5); return; }

	// --- LÓGICA DE OBJETOS INTERACTIVOS ---
	glm::vec2 pPos = player->getPosition();
	glm::vec2 pSize = glm::vec2(16, 32); // Hitbox completa (para llaves y enemigos)

	// Creamos una hitbox reducida solo para la mitad inferior de Bugs Bunny (16x16)
	glm::vec2 feetPos = glm::vec2(pPos.x, pPos.y + 16.0f);
	glm::vec2 feetSize = glm::vec2(16, 16);

	// --- LÓGICA DE OBJETOS INTERACTIVOS (POLIMORFISMO) ---
	for (int i = 0; i < entities.size(); i++) {
		// Llamamos al update genérico. Si es llave, sumará llave. Si es puerta, devolverá destino.
		int destination = entities[i]->update(deltaTime, player, teleportCooldown, entities);
		
		if (destination > 0) {
			// 1. ¿Es una puerta de SALIDA UNIVERSAL (99)?
			if (destination == 99) {
				destination = parentLevel; // Volvemos al nivel del que veníamos
				isReturningFromSubroom = true;
			}
			// 2. ¿Es una puerta de ENTRADA a un subnivel (100+)?
			else if (destination >= 100) {
				parentLevel = currentLevel; // ¡Guardamos de dónde venimos!
				savedPlayerPos = player->getPosition();
			}
			// 3. ¿Viaje a un Nivel Principal NUEVO? (Ej: terminar el Nivel 1 y pasar al 2)
			// ¡EL FIX! Solo borramos las llaves si NO es el nivel actual y NO es el nivel padre
			else if (destination != currentLevel && destination != parentLevel) {
				player->setKeys(0);
				// (Opcional) Si quieres que las bombas y relojes también se reseteen al pasar de mundo:
				// player->setBombs(0); player->setClocks(0);

				entitiesMemory.clear();
				entitiesPosXMemory.clear();
			}

			teleportCooldown = 500.0f;
			loadLevel(destination);
			return; // Cortamos el update
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	// Si estamos en CUALQUIERA de las pantallas de menú
	if (currentState == MENU || currentState == INSTRUCTIONS || currentState == LEVEL_MENU || currentState == CREDITS) {
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

		// Dibujamos el que corresponda
		if (currentState == MENU) menuSprite->render();
		else if (currentState == INSTRUCTIONS) instructionsSprite->render();
		else if (currentState == LEVEL_MENU) levelMenuSprite->render();
		else if (currentState == CREDITS) creditsSprite->render();
	}

	else {//Estamos en PLAYING, así que dibujamos el mapa, el jugador, los enemigos y el HUD

		// --- L{ÓGICA DE LA CÁMARA ---
		float zoom = 2.0f; // Tu nivel de zoom
		float screenAncho = float(SCREEN_WIDTH) / zoom;
		float screenAlto = float(SCREEN_HEIGHT) / zoom;

		// Obtenemos la posición del jugador
		glm::vec2 pos = player->getPosition();
		float playerCentroX = pos.x + SCREEN_X + 8.0f;
		float playerCentroY = pos.y + SCREEN_Y + 8.0f;

		// Calculamos la esquina superior izquierda de la cámara
		float camX = playerCentroX - (screenAncho / 2.0f);
		float camY = playerCentroY - (screenAlto / 2.0f);

		// TOPES DE LA CÁMARA DINÁMICOS
		glm::ivec2 currentMapSize = map->getMapSize();
		float mapWidthPixels = (currentMapSize.x * 16.0f) + SCREEN_X;
		float mapHeightPixels = (currentMapSize.y * 16.0f) + SCREEN_Y;

		float maxCamX = mapWidthPixels - screenAncho;
		float maxCamY = mapHeightPixels - screenAlto;

		// Obligamos a la cámara a quedarse dentro del mapa
		camX = std::max(0.0f, std::min(camX, maxCamX));
		camY = std::max(0.0f, std::min(camY, maxCamY));

		// LA SOLUCIÓN: Creamos una nueva matriz de proyección centrada en la cámara
		// glm::ortho(Izquierda, Derecha, Abajo, Arriba)
		glm::mat4 cameraProjection = glm::ortho(camX, camX + screenAncho, camY + screenAlto, camY);

		// Mandamos los datos a la tarjeta gráfica
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", cameraProjection); // Usamos la cámara aquí
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

		// Dejamos el modelview tranquilo con la matriz identidad
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

		map->render();
		if (!bLoading) {
			for (int i = 0; i < entities.size(); i++) {
				entities[i]->render();
			}
		}
		player->render();

		// ==========================================
		// 2. DIBUJAR EL HUD (INTERFAZ ESTÁTICA)
		// ==========================================

		// A) Restablecemos la cámara original para que el HUD se quede fijo en pantalla
		texProgram.setUniformMatrix4f("projection", projection);

		float hudY = 8.0f; // Margen superior

		// --- B) DIBUJAR VIDAS (Arriba a la Izquierda) ---
		float vidasX = 16.0f;
		hudIconFace->setPosition(glm::vec2(vidasX, hudY));
		hudIconFace->render();

		// Dibujar el número de vidas
		int vidasJugador = player->getLives();
		hudNumber->changeAnimation(vidasJugador);
		hudNumber->setPosition(glm::vec2(vidasX + 20.0f, hudY));
		hudNumber->render();

		// --- C) DIBUJAR LLAVES (Arriba a la Derecha) ---
		float llavesX = screenAncho - 64.0f;
		hudIconKey->setPosition(glm::vec2(llavesX, hudY));
		hudIconKey->render();

		int llavesJugador = player->getKeys();

		// Separar decenas y unidades
		int decenas = llavesJugador / 10;
		int unidades = llavesJugador % 10;

		// Dibujar las Decenas (Ej: el '1' del '15')
		hudNumber->changeAnimation(decenas);
		hudNumber->setPosition(glm::vec2(llavesX + 20.0f, hudY));
		hudNumber->render();

		// Dibujar las Unidades (Ej: el '5' del '15')
		hudNumber->changeAnimation(unidades);
		hudNumber->setPosition(glm::vec2(llavesX + 32.0f, hudY)); // 12 píxeles a la derecha
		hudNumber->render();

		// --- D) DIBUJAR RELOJES ---
		float relojesX = llavesX - 64.0f;
		hudIconClock->setPosition(glm::vec2(relojesX, hudY));
		hudIconClock->render();

		int relojesJugador = player->getClocks();

		//Separar decenas y unidades
		int decRelojes = relojesJugador / 10;
		int uniRelojes = relojesJugador % 10;

		//Dibujar las Decenas
		hudNumber->changeAnimation(decRelojes);
		hudNumber->setPosition(glm::vec2(relojesX + 20.0f, hudY));
		hudNumber->render();

		//Dibujar las Unidades
		hudNumber->changeAnimation(uniRelojes);
		hudNumber->setPosition(glm::vec2(relojesX + 32.0f, hudY));
		hudNumber->render();

		//--- E) DIBUJAR RELOJES ---
		//Dibujar el icono de la bomba
		if (hudIconBomb != NULL) {
			hudIconBomb->setPosition(glm::vec2(120, hudY));
			hudIconBomb->render();
		}
		//Dibujar el número de bombas que tiene el jugador
		if (hudNumber != NULL && player != NULL) {
			//Le pasamos la cantidad de bombas a la animación del número (asumiendo que la anim 0 = '0', anim 1 = '1', etc.)
			hudNumber->changeAnimation(player->getBombs());
			//Ponemos el número un poquito a la derecha del icono de la bomba
			hudNumber->setPosition(glm::vec2(140, hudY));
			hudNumber->render();
		}
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

//Carga un nivel concreto, eliminando el mapa anterior si existía y creando uno nuevo a partir del archivo correspondiente al número de nivel. También reposiciona al jugador al inicio del nivel.
void Scene::loadLevel(int levelNumber)
{
	
	if (levelNumber == 6) {
		currentState = CREDITS;
		return;
	}
	bLoading = true;

	// === PASO 1: GUARDAR EL ESTADO ===
	if (currentLevel != 0) {
		std::vector<EntityMemory> mem;
		for (int i = 0; i < entities.size(); i++) {
			EntityMemory em;
			// EL FIX DEL ENEMIGO ZOMBIE: Si está muriendo, lo guardamos como inactivo (muerto)
			em.active = entities[i]->isActive() && !entities[i]->isEntityDying();
			em.state = entities[i]->getState();
			em.posX = entities[i]->getPos().x;
			em.posY = entities[i]->getPos().y; // ¡NUEVO! Guardamos también la altura
			em.direction = entities[i]->getDirection(); // Guardamos su dirección
			mem.push_back(em);
		}
		entitiesMemory[currentLevel] = mem;
	}

	// Limpieza de memoria
	if (map != NULL) {
		delete map;
		map = NULL;
	}
	clearObjects();

	// Variables para el Enrutador
	string levelFile, objectsFile;
	int spawnTileX = 1;  // Posición por defecto
	int spawnTileY = 23;

	// Selección de archivo según el número de nivel
	switch (levelNumber) {
		// --- NIVELES PRINCIPALES ---
	case 1:
		levelFile = "levels/level01.txt";
		objectsFile = "levels/level01_objects.txt";
		spawnTileX = 1; spawnTileY = 23;
		break;
	case 2:
		levelFile = "levels/level02.txt";
		objectsFile = "levels/level02_objects.txt";
		spawnTileX = 0; spawnTileY = 7; // Ajusta el spawn del nivel 2
		break;
	case 3:
		levelFile = "levels/level03.txt";
		objectsFile = "levels/level03_objects.txt";
		spawnTileX = 0; spawnTileY = 23;
		break;
	case 4:
		levelFile = "levels/level04.txt";
		objectsFile = "levels/level04_objects.txt";
			spawnTileX = 25; spawnTileY = 23;
		break;
	case 5:
		levelFile = "levels/level05.txt";
		objectsFile = "levels/level05_objects.txt";
		spawnTileX = 5; spawnTileY = 23;
		break;

		// --- SUB-HABITACIONES ---
	case 101:
	case 102:
	case 103:
	case 201:
	case 202:
	case 301:
	case 302:
	case 402:
	case 404:
	case 406:
	case 501:
	case 503:
	case 505:
	case 507:
		levelFile = "levels/subnivel_generico_llave_1.txt";
		objectsFile = "levels/subnivel_generico_llave_1_objects.txt";
		spawnTileX = 4; spawnTileY = 7; // Spawn específico para este subnivel
		break;

	case 104:
	case 105:
	case 106:
	case 107:
	case 203:
	case 204:
	case 205:
	case 304:
	case 305:
	case 306:
	case 401:
	case 403:
	case 405:
	case 509:
	case 508:
	case 511:
	case 502:
	case 506:
		levelFile = "levels/subnivel_generico_llave_2.txt";
		objectsFile = "levels/subnivel_generico_llave_2_objects.txt";
		spawnTileX = 1; spawnTileY = 9; // Otro spawn distinto para este subnivel
		break;
	case 206:
	case 303:
	case 510:
	case 504:
		levelFile = "levels/subnivel_generico_bomba_1.txt";
		objectsFile = "levels/subnivel_generico_bomba_1_objects.txt";
		spawnTileX = 7; spawnTileY = 11; // Otro spawn distinto para este subnivel
		break;

		// Si hay un error, cargamos el nivel 1 por defecto
	default:
		levelFile = "levels/level01.txt";
		objectsFile = "levels/level01_objects.txt";
		spawnTileX = 1; spawnTileY = 23;
		break;
	}

	// Carga del nuevo mapa
	map = TileMap::createTileMap(levelFile, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	// 4. LECTURA DINÁMICA DE OBJETOS
	ifstream finObj;
	finObj.open(objectsFile.c_str());
	if (finObj.is_open())
	{
		string header, spritesheetPath;
		float sheetWidth, sheetHeight;

		finObj >> header >> spritesheetPath >> sheetWidth >> sheetHeight;
		itemsSpritesheet.loadFromFile(spritesheetPath, TEXTURE_PIXEL_FORMAT_RGBA);

		string type;
		float sizeX, sizeY, tileX, tileY, spritePixelX, spritePixelY;
		int actionValue;
		int extraValue;

		// ¡RECUERDA! Revisa que los .txt de los subniveles tengan 9 datos por línea.
		while (finObj >> type >> sizeX >> sizeY >> tileX >> tileY >> spritePixelX >> spritePixelY >> actionValue >> extraValue)
		{
			float pixelSueloY = (tileY * 16.0f) + 16.0f;
			glm::vec2 objPos = glm::vec2(tileX * 16.0f, pixelSueloY - sizeY);
			glm::vec2 objSize = glm::vec2(sizeX, sizeY);

			Sprite* newSprite = NULL;
			if (type != "ENEMY_PIOLIN" && type != "ENEMY_PERRO" && type != "ENEMY_PATO") {
				float uvWidth = sizeX / sheetWidth;
				float uvHeight = sizeY / sheetHeight;
				float uvX = spritePixelX / sheetWidth;
				float uvY = spritePixelY / sheetHeight;

				newSprite = Sprite::createSprite(objSize, glm::vec2(uvWidth, uvHeight), &itemsSpritesheet, &texProgram);

				if (type == "DOOR" || type == "DOOR_FINAL") {
					newSprite->setNumberAnimations(2);
					newSprite->setAnimationSpeed(0, 1);
					newSprite->addKeyframe(0, glm::vec2(uvX, uvY));
					newSprite->setAnimationSpeed(1, 1);
					newSprite->addKeyframe(1, glm::vec2(uvX + (uvWidth * 2.0f), uvY));
				}
				else if (type == "BOMB") {
					newSprite->setNumberAnimations(2);
					newSprite->setAnimationSpeed(0, 1);
					newSprite->addKeyframe(0, glm::vec2(uvX, uvY));
					newSprite->setAnimationSpeed(1, 1);
					newSprite->addKeyframe(0, glm::vec2(uvX, uvY));
					newSprite->addKeyframe(1, glm::vec2(uvX + (uvWidth * 1.0f), uvY));// encendida
				}

				else {
					newSprite->setNumberAnimations(1);
					newSprite->setAnimationSpeed(0, 1);
					newSprite->addKeyframe(0, glm::vec2(uvX, uvY));
				}
				newSprite->changeAnimation(0);
				newSprite->setPosition(glm::vec2(SCREEN_X + objPos.x, SCREEN_Y + objPos.y));
			}

			// INSTANCIAMOS LA CLASE CORRECTA
			if (type == "KEY") {
				Key* k = new Key();
				k->init(objPos, objSize, newSprite);
				entities.push_back(k);
			}
			else if (type == "DOOR") {
				Door* d = new Door();
				d->init(objPos, objSize, newSprite, actionValue);
				entities.push_back(d);
			}
			else if (type == "DOOR_FINAL") {
				FinalDoor* fd = new FinalDoor();
				fd->init(objPos, objSize, newSprite, actionValue, extraValue);
				entities.push_back(fd);
			}
			else if (type == "ENEMY_PIOLIN") {
				Piolin* p = new Piolin();
				p->setPos(objPos);
				p->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, map);
				entities.push_back(p);
			}
			else if (type == "WEIGHT") {
				Weight* w = new Weight();
				// Le pasamos el mapa y el desplazamiento (SCREEN_X, SCREEN_Y)
				w->init(objPos, objSize, newSprite, map, glm::vec2(SCREEN_X, SCREEN_Y));
				entities.push_back(w);
			}
			else if (type == "CLOCK") {
				Clock* c = new Clock();
				c->init(objPos, objSize, newSprite);
				entities.push_back(c);
			}
			else if (type == "BOMB") {
				Bomb* b = new Bomb();
				b->init(objPos, objSize, newSprite, Bomb::COLLECTIBLE, map, glm::vec2(SCREEN_X, SCREEN_Y));
				entities.push_back(b);
			}
			else if (type == "SPEED_BOOST") {
				SpeedBoost* sb = new SpeedBoost();
				sb->init(objPos, objSize, newSprite);
				entities.push_back(sb);
			}
			else if (type == "ENEMY_PERRO") {
				Perro* p = new Perro();
				p->setPos(objPos);
				p->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, map);
				entities.push_back(p);
			}
			else if (type == "ENEMY_PATO") {
				Pato* duck = new Pato();
				duck->setPos(objPos);
				duck->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, map);
				entities.push_back(duck);
			}
		}
		finObj.close();
	}

	// === PASO 2: RESTAURAR EL ESTADO ===
	if (entitiesMemory.find(levelNumber) != entitiesMemory.end()) {
		for (int i = 0; i < entities.size(); i++) {
			entities[i]->setActive(entitiesMemory[levelNumber][i].active);
			entities[i]->setState(entitiesMemory[levelNumber][i].state);

			// EL FIX DE LA DIRECCIÓN: Le devolvemos la dirección que tenía
			entities[i]->setDirection(entitiesMemory[levelNumber][i].direction);

			if (entities[i]->isActive()) {
				glm::vec2 oldPos = entities[i]->getPos();
				oldPos.x = entitiesMemory[levelNumber][i].posX;
				oldPos.y = entitiesMemory[levelNumber][i].posY; // ¡NUEVO! Le devolvemos su altura exacta
				entities[i]->setPos(oldPos);
			}
		}
	}

	// 5. Actualizar al jugador usando el Spawn Dinámico
	if (player != NULL) {
		player->setTileMap(map);

		// Si volvemos de una sala secreta, restauramos posición frente a la puerta
		if (levelNumber < 100 && isReturningFromSubroom) {
			player->setPosition(savedPlayerPos);
			isReturningFromSubroom = false;
		}
		// Si es un nivel nuevo o entramos a una sala, usamos las coordenadas del switch
		else {
			// Ponemos al jugador en la entrada
			player->setPosition(glm::vec2(spawnTileX * map->getTileSize(), spawnTileY * map->getTileSize()));

			// ¡EL FIX! Solo reseteamos el inventario si estamos cargando un Nivel Principal (1, 2, 3...)
			// Si estamos entrando a un subnivel (100, 101...), respetamos lo que lleve en los bolsillos
			if (levelNumber < 100) {
				player->setKeys(0);
				player->setClocks(0);
				player->setBombs(0);
				player->stopSpeedBoost();
			}
		}
	}

	currentLevel = levelNumber;
	bLoading = false;
}

void Scene::spawnBomb(const glm::vec2& pos) {
	Bomb* b = new Bomb();

	float frameWidth = 1.0f / 17.0f; // El spritesheet tiene 17 columnas
	float frameHeight = 1.0f / 7.0f; // El spritesheet tiene 7 filas

	// USAMOS itemsSpritesheet QUE YA EXISTE EN LA ESCENA, NO CREAMOS UNA NUEVA
	Sprite* bombSprite = Sprite::createSprite(glm::vec2(16, 16), glm::vec2(frameWidth, frameHeight), &itemsSpritesheet, &texProgram);
	bombSprite->setNumberAnimations(2);

	// Animación 0: Apagado
	bombSprite->setAnimationSpeed(0, 8); // ¡Corregido al 0!
	bombSprite->addKeyframe(0, glm::vec2(frameWidth * 2, frameHeight * 0));

	// Animación 1: Encendido (Parpadeo)
	bombSprite->setAnimationSpeed(1, 8);
	bombSprite->addKeyframe(1, glm::vec2(frameWidth * 2, frameHeight * 0));
	bombSprite->addKeyframe(1, glm::vec2(frameWidth * 3, frameHeight * 0)); // O el frame que sea para que parpadee

	// Inicializamos la bomba y la añadimos a las entidades de la escena
	bombSprite->setPosition(glm::vec2(SCREEN_X + pos.x, SCREEN_Y + pos.y));
	b->init(pos, glm::vec2(16, 16), bombSprite, Bomb::DROPPED, map, glm::vec2(SCREEN_X, SCREEN_Y));
	entities.push_back(b);
	std::cout << "\n[DEBUG] Bomba lanzada en posición (" << pos.x << ", " << pos.y << "). Bombas restantes: " << player->getBombs() << std::endl;
}

void Scene::clearObjects() {
	for (size_t i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	entities.clear();
}