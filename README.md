# 🐰 Bugs Bunny Crazy Castle 3 - C++ / OpenGL Replica 🏰

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl)
![Visual Studio](https://img.shields.io/badge/Visual_Studio-5C2D91?style=for-the-badge&logo=visual%20studio&logoColor=white)

🌍 **Choose your language / Elige tu idioma:**
* [🇬🇧 English Version](#english-version)
* [🇪🇸 Versión en Español](#versión-en-español)

---

<a id="english-version"></a>
## 🇬🇧 English Version

2D replica of the classic *Bugs Bunny Crazy Castle 3* for GameBoy Color. Developed in **C++** using **OpenGL** as a university project for the Videogames (VJ) course at FIB (UPC).

## 🎮 Gameplay Demo

<video src="demo.mp4" controls="controls" style="max-width: 100%;">
  Your browser does not support the video tag.
</video>

---

## 🚀 About the Project

**[EN]** The player controls Bugs Bunny through the castle rooms to collect all the keys and unlock the door to the next level, while avoiding or eliminating Looney Tunes enemies.  

### ✨ Key Features
* **Custom Engine:** 2D Tile-Based engine built from scratch (C++, OpenGL, GLFW, GLM).
* **FSM (Finite State Machine):** 4 screens managed smoothly (Menu, Instructions, Level Selector, Gameplay, Credits).
* **Advanced AI:** * **Daffy Duck (Pato Lucas):** Vertical & horizontal pathfinding (climbs vines, jumps, falls).
    * **Tweety & Dog (Piolín y Perro):** Edge and wall detection patrol.
* **Physics:** Custom gravity, pushable weights to crush enemies, trampolines, and sticky floors.
* **Items:** Real-time HUD, placeable bombs, and clocks to freeze the AI.

---

## 📂 Structure

```text
📦 Bugs-Bunny-Crazy-Castle-3
 ┣ 📂 Binari           # Executable and required .dlls
 ┃ ┣ 📂 audio          # Music
 ┃ ┣ 📂 images         # UI, spritesheets and backgrounds
 ┃ ┣ 📂 levels         # Tilemap configuration (.txt)
 ┃ ┗ 📂 shaders        # OpenGL Shaders
 ┣ 📂 Projecte         # Source Code (.cpp & .h)
 ┃ ┣ 📜 Scene.cpp      # Game loop & FSM
 ┃ ┣ 📜 Player.cpp     # Bugs Bunny controller & physics
 ┃ ┣ 📜 Entity.cpp     # Polymorphic base class
 ┃ ┣ 📜 Pato.cpp       # Daffy Duck advanced AI
 ┃ ┗ 📜 ...            # Bomb, Key, Weight, Clock, Door, etc.
 ┗ 📜 demo.mp4         # Gameplay video
```

## 🕹️ Controls

| Action | Key |
| :--- | :--- |
| **Move** | `Left / Right Arrows` ⬅️ ➡️ |
| **Climb** | `Up / Down Arrows` ⬆️ ⬇️ |
| **Enter Doors** | `Up Arrow` ⬆️ |
| **Drop Bomb** | `B` |
| **Use Clock** | `R` |
| **Select Menus** | `ENTER` |

### 🐛 Debug Keys
* `G`: God Mode (Invincibility).
* `K`: Add 99 keys instantly.
* `1 to 5`: Load specific level.


<a id="versión-en-español"></a>
## 🇪🇸 Versión en Español

Réplica en 2D del clásico juego *Bugs Bunny Crazy Castle 3* de GameBoy Color. Desarrollado en **C++** utilizando **OpenGL** como proyecto universitario para la asignatura de Videojuegos (VJ) de la FIB (UPC).

## 🎮 Demo del Juego

<video src="demo.mp4" controls="controls" style="max-width: 100%;">
  Tu navegador no soporta la etiqueta de vídeo.
</video>

---

## 🚀 Sobre el Proyecto

**[ES]** El jugador controla a Bugs Bunny por las salas del castillo para recoger todas las llaves y abrir la puerta al siguiente nivel, mientras esquiva o elimina a los enemigos de Looney Tunes.  

### ✨ Funcionalidades Destacadas
* **Motor Propio:** Motor 2D Tile-Based creado desde cero (C++, OpenGL, GLFW, GLM).
* **FSM (Máquina de Estados):** Gestión fluida de 4 pantallas (Menú, Instrucciones, Selector de Niveles, Juego, Créditos).
* **IA Avanzada:** * **Pato Lucas:** Pathfinding vertical y horizontal (escala lianas, salta, cae).
    * **Piolín y Perro:** Patrulla con detección de bordes y paredes.
* **Físicas:** Gravedad propia, pesas que se pueden empujar para aplastar enemigos, trampolines y suelos pegajosos.
* **Objetos:** HUD en tiempo real, bombas colocables y relojes para congelar a la IA.

---

## 📂 Estructura

```text
📦 Bugs-Bunny-Crazy-Castle-3
 ┣ 📂 Binari           # Ejecutable y DLLs necesarias
 ┃ ┣ 📂 audio          # Música
 ┃ ┣ 📂 images         # Interfaz, spritesheets y fondos
 ┃ ┣ 📂 levels         # Configuración de mapas de tiles (.txt)
 ┃ ┗ 📂 shaders        # Shaders de OpenGL
 ┣ 📂 Projecte         # Código Fuente (.cpp & .h)
 ┃ ┣ 📜 Scene.cpp      # Bucle del juego y FSM
 ┃ ┣ 📜 Player.cpp     # Control y físicas de Bugs Bunny
 ┃ ┣ 📜 Entity.cpp     # Clase base polimórfica
 ┃ ┣ 📜 Pato.cpp       # IA avanzada del Pato Lucas
 ┃ ┗ 📜 ...            # Bomba, Llave, Pesa, Reloj, Puerta, etc.
 ┗ 📜 demo.mp4         # Vídeo del juego
```
## 🕹️ Controles

| Acción | Tecla |
| :--- | :--- |
| **Moverse** | `Flechas Izquierda / Derecha` ⬅️ ➡️ |
| **Escalar** | `Flechas Arriba / Abajo` ⬆️ ⬇️ |
| **Entrar por Puertas** | `Flecha Arriba` ⬆️ |
| **Dejar Bomba** | `B` |
| **Usar Reloj** | `R` |
| **Navegar Menús** | `ENTER` |

### 🐛 Teclas de Debug
* `G`: Modo Dios (Invulnerabilidad).
* `K`: Añadir 99 llaves al instante.
* `1 al 5`: Cargar nivel específico.
