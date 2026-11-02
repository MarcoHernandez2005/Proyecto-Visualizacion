# Visualizador de Modelos OBJ y Función Matemática
**Universidad Nacional Autónoma de México**  
**Facultad de Ciencias**  
Materia: Visualización (Grupo 7018 - Semestre 2027-1)  

**Desarrollado por:** 
- Marco Antonio Hernández Torres
- Libni Jael Morales Silva

## Descripción del Proyecto
Visualizador 3D en C++ utilizando OpenGL, GLFW, GLEW y GLM. El programa centra los modelos dinámicamente calculando su Bounding Box e implementa un shader que interpola el color de azul a rojo basándose en la altura (eje Y). 

Se cumplieron los requisitos extras:
1. **Interfaz Gráfica:** Dear ImGui para seleccionar modelos, modos de dibujo y transformaciones.
2. **Cámara Orbital:** Navegación 3D interactiva con el ratón.
3. **Plano Matemático:** Evaluación de $f(x,z)=cos(x)+sin(z)$ en $[-2\pi,2\pi]\times[-2\pi,2\pi]$.

## Cómo compilar y ejecutar
**Requisitos:** MSYS2 (UCRT64), CMake y Ninja.

1. `mkdir build && cd build`
2. `cmake -G "Ninja" ..`
3. `cmake --build .`
4. `./ProyectoMedioSemestre.exe`