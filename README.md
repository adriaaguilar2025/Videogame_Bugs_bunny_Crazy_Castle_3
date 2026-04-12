# 🐰 Bugs Bunny Crazy Castle 3 - C++ / OpenGL Replica 🏰
Sergi Malaguilla Bombín & Adria Aguilar Garcia
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl)
![Visual Studio](https://img.shields.io/badge/Visual_Studio-5C2D91?style=for-the-badge&logo=visual%20studio&logoColor=white)

Réplica en 2D del clásico juego *Bugs Bunny Crazy Castle 3* de GameBoy Color. Desarrollado en **C++** utilizando **OpenGL** como proyecto universitario para la asignatura de Videojuegos (VJ) de la FIB (UPC).

## 🎮 Demo del Juego

> **Nota:** Haz clic en el vídeo para reproducirlo.

<video src="demo.mp4" controls="controls" style="max-width: 100%;">
  Tu navegador no soporta la etiqueta de vídeo.
</video>

---

## 🚀 Sobre el Proyecto

El jugador controla a Bugs Bunny a través de las salas del castillo con un único objetivo: recoger todas las llaves para abrir la puerta al siguiente nivel, mientras esquiva o elimina a diferentes enemigos del universo Looney Tunes usando su ingenio y los objetos del entorno.

### ✨ Funcionalidades Destacadas
- **Arquitectura de Motor Propio:** Motor 2D Tile-Based creado desde cero con C++, OpenGL, GLFW y GLM.
- **Máquina de Estados (FSM):** Gestión fluida entre 4 pantallas principales (Menú, Instrucciones, Selector de Niveles, Juego y Créditos).
- **IA de Enemigos:** - **Pato Lucas:** Enemigo avanzado con *pathfinding* vertical y horizontal. Puede escalar lianas, saltar y caer por agujeros para perseguir al jugador.
  - **Piolín y Perro:** Enemigos de patrulla con detección de colisiones y bordes.
- **Interacciones y Físicas:** - Sistema de gravedad personalizado.
  - Pesas dinámicas que el jugador puede empujar para aplastar enemigos.
  - Suelos especiales (Trampolines y Suelos pegajosos).
- **Inventario y Objetos:** HUD en pantalla que actualiza en tiempo real vidas, llaves, bombas (colocables) y relojes (congelan a la IA).

---

## 📂 Estructura del Proyecto

El código sigue el paradigma de Programación Orientada a Objetos (OOP) haciendo un uso extensivo del polimorfismo para las entidades del juego.

```text
📦 Bugs-Bunny-Crazy-Castle-3
 ┣ 📂 Binari           # Archivos compilados (.exe) y .dll necesarias
 ┃ ┣ 📂 audio          # Efectos de sonido y música
 ┃ ┣ 📂 images         # Spritesheets y fondos de pantalla
 ┃ ┣ 📂 levels         # Mapas de los niveles en formato .txt (Tilemaps)
 ┃ ┗ 📂 shaders        # Vertex y Fragment shaders de OpenGL
 ┣ 📂 Projecte         # Código fuente (.cpp y .h)
 ┃ ┣ 📜 Scene.cpp      # Bucle principal, renderizado y control de cámara
 ┃ ┣ 📜 Player.cpp     # Físicas y control de Bugs Bunny
 ┃ ┣ 📜 Entity.cpp     # Clase padre polimórfica
 ┃ ┣ 📜 Pato.cpp       # IA avanzada del enemigo
 ┃ ┗ 📜 ...            # Resto de clases (Bomb, Key, Weight, etc.)
 ┗ 📜 demo.mp4         # Vídeo demostrativo
