# 🐰 Bugs Bunny Crazy Castle 3 - C++ / OpenGL Replica 🏰

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl)
![Visual Studio](https://img.shields.io/badge/Visual_Studio-5C2D91?style=for-the-badge&logo=visual%20studio&logoColor=white)

**[EN]** 2D replica of the classic *Bugs Bunny Crazy Castle 3* for GameBoy Color. Developed in **C++** using **OpenGL** as a university project for the Videogames (VJ) course at FIB (UPC).  
**[ES]** Réplica en 2D del clásico juego *Bugs Bunny Crazy Castle 3* de GameBoy Color. Desarrollado en **C++** utilizando **OpenGL** como proyecto universitario para la asignatura de Videojuegos (VJ) de la FIB (UPC).

---

## 🎮 Gameplay Demo / Demo del Juego

> **[EN]** Note: Drag and drop your `demo.mp4` file here when editing this README on GitHub.  
> **[ES]** Nota: Arrastra y suelta tu archivo `demo.mp4` aquí al editar este README en GitHub.

<video src="demo.mp4" controls="controls" style="max-width: 100%;">
  Your browser does not support the video tag.
</video>

---

## 🚀 About the Project / Sobre el Proyecto

**[EN]** The player controls Bugs Bunny through the castle rooms to collect all the keys and unlock the door to the next level, while avoiding or eliminating Looney Tunes enemies.  
**[ES]** El jugador controla a Bugs Bunny por el castillo para recoger todas las llaves y abrir la puerta al siguiente nivel, esquivando o eliminando a los enemigos de Looney Tunes.

### ✨ Key Features / Funcionalidades Destacadas
* **Custom Engine / Motor Propio:** 2D Tile-Based engine built from scratch (C++, OpenGL, GLFW, GLM).
* **FSM (Finite State Machine):** 4 screens managed smoothly (Menu, Instructions, Level Selector, Gameplay, Credits).
* **Advanced AI / IA Avanzada:** * **Daffy Duck (Pato Lucas):** Vertical & horizontal pathfinding (climbs vines, jumps, falls). / *Pathfinding vertical y horizontal (escala, salta, cae).*
    * **Tweety & Dog (Piolín y Perro):** Edge and wall detection patrol. / *Patrulla con detección de bordes y paredes.*
* **Physics / Físicas:** Custom gravity, pushable weights to crush enemies, trampolines, and sticky floors. / *Gravedad propia, pesas que se pueden empujar, trampolines y suelos pegajosos.*
* **Items:** Real-time HUD, placeable bombs, and clocks to freeze the AI. / *HUD en tiempo real, bombas colocables y relojes para congelar a la IA.*

---

## 📂 Structure / Estructura del Proyecto

```text
📦 Bugs-Bunny-Crazy-Castle-3
 ┣ 📂 Binari           # Executable and required .dlls / Ejecutable y DLLs
 ┃ ┣ 📂 audio          # Music and sound effects / Música y efectos
 ┃ ┣ 📂 images         # UI, spritesheets and backgrounds / Texturas
 ┃ ┣ 📂 levels         # Tilemap configuration (.txt) / Mapas de niveles
 ┃ ┗ 📂 shaders        # OpenGL Shaders
 ┣ 📂 Projecte         # Source Code / Código Fuente (.cpp & .h)
 ┃ ┣ 📜 Scene.cpp      # Game loop & FSM
 ┃ ┣ 📜 Player.cpp     # Bugs Bunny controller & physics
 ┃ ┣ 📜 Entity.cpp     # Polymorphic base class / Clase padre polimórfica
 ┃ ┣ 📜 Pato.cpp       # Daffy Duck advanced AI / IA avanzada
 ┃ ┗ 📜 ...            # Bomb, Key, Weight, Clock, Door, etc.
 ┗ 📜 demo.mp4         # Gameplay video
```

## 🕹️ Controls / Controles

| Action / Acción | Key / Tecla |
| :--- | :--- |
| **Move / Moverse** | `Left / Right Arrows` ⬅️ ➡️ |
| **Climb / Escalar Lianas** | `Up / Down Arrows` ⬆️ ⬇️ |
| **Enter Doors / Entrar Puertas** | `Up Arrow` ⬆️ |
| **Drop Bomb / Dejar Bomba** | `B` |
| **Use Clock / Usar Reloj** | `R` |
| **Select / Navegar Menús** | `ENTER` |

### 🐛 Debug Keys / Teclas de Debug
* `G`: God Mode (Invincibility / Invulnerabilidad).
* `K`: Add 99 keys instantly / Añadir 99 llaves.
* `1 to 5`: Load specific level / Cargar nivel específico.
