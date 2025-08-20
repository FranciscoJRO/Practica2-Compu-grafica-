# Práctica 2 – Pixel Art con OpenGL

**Alumno:** Francisco Javier Reynoso Ortega  
**No. de cuenta:**   
**Fecha de entrega:** 10/08/2025  

## 📌 Descripción
Este proyecto dibuja un **sprite tipo pixel-art (Spider-Man)** en una ventana de OpenGL usando **GLFW + GLEW**.  
Cada celda del arreglo `S` representa un pixel del sprite y se rasteriza como dos triángulos (quad).  
La paleta `PALETTE` asigna colores a cada número.

## 🎨 Paleta de colores
- **0** → Fondo (gris claro, no se dibuja geometría)  
- **1** → Negro  
- **2** → Rojo  
- **3** → Blanco  
- **4** → Azul  

## ⚙️ Requisitos
- Compilador C++11 o superior  
- [GLFW](https://www.glfw.org/)  
- [GLEW](http://glew.sourceforge.net/)  
- OpenGL 3.3 o superior  

## 🚀 Compilación y ejecución

### En Linux/Mac:
```bash
g++ Main_Dibujo2D.cpp Shader.cpp -lglfw -lGLEW -lGL -o practica2
./practica2
