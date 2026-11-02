cmake_minimum_required(VERSION 3.10)
project(ProyectoMedioSemestre)

set(CMAKE_CXX_STANDARD 17)

# Rutas de inclusión (Asegúrate de que GLM tenga la subcarpeta glm/)
include_directories(include)
include_directories(include/GLFW)
include_directories(include/GLM) 
include_directories(include/imgui)

set(SOURCES 
    src/main.cpp
    src/Scene.cpp
    src/Model.cpp
    src/ShaderProgram.cpp
    src/OrbitCamera.cpp
    src/WindowGL.cpp
    # ImGui
    include/imgui/imgui.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/imgui_impl_glfw.cpp
    include/imgui/imgui_impl_opengl3.cpp
)

add_executable(ProyectoMedioSemestre ${SOURCES})

target_link_libraries(ProyectoMedioSemestre 
    opengl32 
    glfw3 
    glew32
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})