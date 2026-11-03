# Visualizador de Modelos OBJ y Función Matemática
**Universidad Nacional Autónoma de México**  
**Facultad de Ciencias**  
Materia: Visualización (Semestre 2027-1)  

**Desarrollado por:** 
- Marco Antonio Hernández Torres
- Libni Jael Morales Silva

## Descripción del Proyecto
Visualizador 3D en C++ utilizando OpenGL, GLFW, GLEW y GLM. El programa centra los modelos dinámicamente calculando su Bounding Box e implementa un shader que interpola el color de azul a rojo basándose en la altura (eje Y). 

Se cumplieron los requisitos extras:
1. **Interfaz Gráfica:** Dear ImGui para seleccionar modelos, modos de dibujo y transformaciones.
2. **Cámara Orbital:** Navegación 3D interactiva con el ratón.
3. **Plano Matemático:** Evaluación de $f(x,z)=\cos(x)+\sin(z)$ en $[-2\pi,2\pi]\times[-2\pi,2\pi]$.

## 📸 Capturas de Pantalla

| Vista 1 | Vista 2 |
| :---: | :---: |
| ![Captura 1](Capturas%20de%20Pantalla/C1.png) | ![Captura 2](Capturas%20de%20Pantalla/C2.png) |
| ![Captura 3](Capturas%20de%20Pantalla/C3.png) | ![Captura 4](Capturas%20de%20Pantalla/C4.png) |
| ![Captura 5](Capturas%20de%20Pantalla/C5.png) | ![Captura 6](Capturas%20de%20Pantalla/C6.png) |
| ![Captura 7](Capturas%20de%20Pantalla/C7.png) | |

## Cómo compilar y ejecutar
**Requisitos:** MSYS2 (UCRT64), CMake y Ninja.

Abre tu terminal (PowerShell o Git Bash) en la raíz del proyecto y ejecuta los siguientes comandos:

```powershell
# 1. Crear y entrar a la carpeta build
mkdir build
cd build

# 2. Configurar el proyecto con CMake y Ninja
cmake -G "Ninja" ..

# 3. Compilar el proyecto
cmake --build .

# 4. Ejecutar el programa (estando dentro de la carpeta build)
.\ProyectoMedioSemestre.exe
