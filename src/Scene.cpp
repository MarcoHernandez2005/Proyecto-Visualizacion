#include "Scene.h"
#include "WindowGL.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "OrbitCamera.h"
#include <iostream>
#include <cmath>
#include <limits>

// Función auxiliar para generar el plano matemático f(x,z) = cos(x) + sin(z)[cite: 5]
static Model* generarPlanoMatematico(ShaderProgram* shader) {
    int N = 100; // 100x100 vértices[cite: 5]
    float minDomain = -2.0f * glm::pi<float>();
    float maxDomain =  2.0f * glm::pi<float>();
    float step = (maxDomain - minDomain) / (N - 1);

    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;

    glm::vec3 minAABB = glm::vec3(minDomain, std::numeric_limits<float>::max(), minDomain);
    glm::vec3 maxAABB = glm::vec3(maxDomain, std::numeric_limits<float>::lowest(), maxDomain);

    for (int i = 0; i < N; ++i) {
        float x = minDomain + i * step;
        for (int j = 0; j < N; ++j) {
            float z = minDomain + j * step;
            float y = std::cos(x) + std::sin(z); // f(x,z) = cos(x) + sin(z)[cite: 5]

            vertices.push_back(glm::vec3(x, y, z));
            minAABB.y = std::min(minAABB.y, y);
            maxAABB.y = std::max(maxAABB.y, y);
        }
    }

    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < N - 1; ++j) {
            GLuint row1 = i * N + j;
            GLuint row2 = (i + 1) * N + j;

            indices.push_back(row1);
            indices.push_back(row2);
            indices.push_back(row1 + 1);

            indices.push_back(row1 + 1);
            indices.push_back(row2);
            indices.push_back(row2 + 1);
        }
    }

    std::cout << "--- Modelo: Plano Funcional [100x100] ---" << std::endl;
    std::cout << "Vertices: " << vertices.size() << std::endl;
    std::cout << "Min Y: " << minAABB.y << std::endl;
    std::cout << "Max Y: " << maxAABB.y << "\n" << std::endl;

    return new Model(shader, vertices, indices, minAABB, maxAABB);
}

// 1. Constructor
Scene::Scene() 
    : m_window(nullptr), m_shaderProgram(nullptr), m_camera(nullptr), m_currentModelIndex(0) {
}

// 2. Destructor
Scene::~Scene() {
    for (Model* model : m_models) {
        delete model;
    }
    m_models.clear();

    delete m_camera;
    delete m_shaderProgram;
    delete m_window;
}

void Scene::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (ImGui::GetIO().WantCaptureMouse) return;

    Scene* instance = static_cast<Scene*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double currX, currY;
        glfwGetCursorPos(window, &currX, &currY);
        instance->m_last = glm::vec2((float)currX, (float)currY);
        instance->m_cameraMovement = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        instance->m_cameraMovement = false;
    }
}

void Scene::init() {
    m_window = new WindowGL();
    glfwSetWindowUserPointer(m_window->getWindow(), this);
    glfwSetMouseButtonCallback(m_window->getWindow(), Scene::mouseCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    m_shaderProgram = new ShaderProgram("shaders/vertex_shader.glsl", "shaders/frag_shader.glsl");

    // Cargar modelos de archivo OBJ
    m_models.push_back(new Model(m_shaderProgram, "obj/bunny.obj"));
    m_models.push_back(new Model(m_shaderProgram, "obj/car.obj"));
    m_models.push_back(new Model(m_shaderProgram, "obj/casa.obj"));
    m_models.push_back(new Model(m_shaderProgram, "obj/cat.obj"));
    m_models.push_back(new Model(m_shaderProgram, "obj/pescado.obj"));
    m_models.push_back(new Model(m_shaderProgram, "obj/Teapot.obj"));

    // Cargar el Plano Matematico (f(x,z) = cos(x) + sin(z))[cite: 5]
    m_models.push_back(generarPlanoMatematico(m_shaderProgram));

    m_camera = new OrbitCamera(glm::vec3(0.5f, 3.0f, 12.0f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
    m_view = m_camera->getMatrix();
    m_projection = glm::perspective(glm::radians(45.0f), m_window->getAspectRatio(), 0.1f, 100.0f);
}

void Scene::render() {
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(m_window->getWindow())) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Panel de Control");
        Model* currentModel = m_models[m_currentModelIndex];
        
        if (ImGui::BeginCombo("Modelo", ("Modelo " + std::to_string(m_currentModelIndex)).c_str())) {
            for (size_t i = 0; i < m_models.size(); i++) {
                bool is_selected = (m_currentModelIndex == static_cast<int>(i));
                std::string label = (i == m_models.size() - 1) ? "Plano f(x,z)" : ("Modelo " + std::to_string(i));
                if (ImGui::Selectable(label.c_str(), is_selected))
                    m_currentModelIndex = static_cast<int>(i);
            }
            ImGui::EndCombo();
        }

        ImGui::RadioButton("Superficie", &currentModel->drawMode, 0); ImGui::SameLine();
        ImGui::RadioButton("Lineas", &currentModel->drawMode, 1); ImGui::SameLine();
        ImGui::RadioButton("Puntos", &currentModel->drawMode, 2);

        ImGui::SliderFloat("Rotacion X", &currentModel->rotX, 0.0f, 6.28f);
        ImGui::SliderFloat("Rotacion Y", &currentModel->rotY, 0.0f, 6.28f);
        ImGui::SliderFloat("Rotacion Z", &currentModel->rotZ, 0.0f, 6.28f);
        ImGui::SliderFloat("Escala", &currentModel->scaleMultiplier, 0.1f, 3.0f);
        ImGui::End();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentModel->renderAxis(m_view, m_projection);
        currentModel->renderModel(m_view, m_projection);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window->getWindow());

        if (m_cameraMovement) {
            double currX, currY;
            glfwGetCursorPos(m_window->getWindow(), &currX, &currY);
            m_current = glm::vec2((float)currX, (float)currY);
            m_camera->rotate(m_last, m_current);
            m_camera->setPos(m_last, m_current);
        }
        m_view = m_camera->getMatrix();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}