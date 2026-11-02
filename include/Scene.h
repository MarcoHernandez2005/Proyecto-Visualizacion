#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <string>

// Declaraciones anticipadas para evitar incluir todo en el header
class WindowGL;
class ShaderProgram;
class Model;
class OrbitCamera;

class Scene {
public:
    Scene();
    ~Scene();
    void init();
    void render();

private:
    WindowGL* m_window = nullptr;
    ShaderProgram* m_shaderProgram = nullptr;
    OrbitCamera* m_camera = nullptr;
    std::vector<Model*> m_models;
    
    glm::mat4 m_view;
    glm::mat4 m_projection;
    int m_currentModelIndex = 0;

    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
    bool m_cameraMovement = false;
    glm::vec2 m_last, m_current;
};