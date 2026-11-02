#include "Model.h"
#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <limits>

Model::Model(ShaderProgram* shader, const std::string& objPath)
    : m_shader(shader), rotX(0.0f), rotY(0.0f), rotZ(0.0f), scaleMultiplier(1.0f), drawMode(0) {
    loadOBJ(objPath);
}

// Constructor para el plano matemático
Model::Model(ShaderProgram* shader, const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices, glm::vec3 minAABB, glm::vec3 maxAABB)
    : m_shader(shader), rotX(0.0f), rotY(0.0f), rotZ(0.0f), scaleMultiplier(1.0f), drawMode(0), m_minAABB(minAABB), m_maxAABB(maxAABB) {
    setupBuffers(vertices, indices);
}

Model::~Model() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_axisVAO);
    glDeleteBuffers(1, &m_axisVBO);
}

void Model::setupBuffers(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices) {
    m_indexCount = static_cast<int>(indices.size());
    m_center = (m_minAABB + m_maxAABB) * 0.5f;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Ejes Coordenados
    float axisVertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &m_axisVAO);
    glGenBuffers(1, &m_axisVBO);

    glBindVertexArray(m_axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Model::loadOBJ(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo OBJ: " << path << std::endl;
        return;
    }

    std::vector<glm::vec3> temp_vertices;
    std::vector<GLuint> indices;

    m_minAABB = glm::vec3(std::numeric_limits<float>::max());
    m_maxAABB = glm::vec3(std::numeric_limits<float>::lowest());

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);

            m_minAABB = glm::min(m_minAABB, vertex);
            m_maxAABB = glm::max(m_maxAABB, vertex);
        }
        else if (prefix == "f") {
            std::string vStr;
            std::vector<GLuint> faceIndices;

            while (ss >> vStr) {
                std::stringstream vSs(vStr);
                std::string indexStr;
                std::getline(vSs, indexStr, '/');

                if (!indexStr.empty()) {
                    int idx = std::stoi(indexStr);
                    if (idx > 0) faceIndices.push_back(static_cast<GLuint>(idx - 1));
                    else if (idx < 0) faceIndices.push_back(static_cast<GLuint>(temp_vertices.size() + idx));
                }
            }

            for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
                indices.push_back(faceIndices[0]);
                indices.push_back(faceIndices[i]);
                indices.push_back(faceIndices[i + 1]);
            }
        }
    }
    file.close();

    std::cout << "--- Modelo: " << path << " ---" << std::endl;
    std::cout << "Vertices: " << temp_vertices.size() << std::endl;
    std::cout << "Min: (" << m_minAABB.x << ", " << m_minAABB.y << ", " << m_minAABB.z << ")" << std::endl;
    std::cout << "Max: (" << m_maxAABB.x << ", " << m_maxAABB.y << ", " << m_maxAABB.z << ")\n" << std::endl;

    setupBuffers(temp_vertices, indices);
}

void Model::renderModel(const glm::mat4& view, const glm::mat4& projection) {
    m_shader->use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, rotX, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotY, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scaleMultiplier));

    m_shader->setMat4x4("model", model);
    m_shader->setMat4x4("view", view);
    m_shader->setMat4x4("projection", projection);
    m_shader->setFloat("minY", m_minAABB.y);
    m_shader->setFloat("maxY", m_maxAABB.y);

    if (drawMode == 0) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (drawMode == 1) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.5f);
    } else if (drawMode == 2) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glPointSize(3.0f);
    }

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Model::renderAxis(const glm::mat4& view, const glm::mat4& projection) {
    m_shader->use();

    glm::mat4 model = glm::mat4(1.0f);
    m_shader->setMat4x4("model", model);
    m_shader->setMat4x4("view", view);
    m_shader->setMat4x4("projection", projection);

    glBindVertexArray(m_axisVAO);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, 6);
    glLineWidth(1.0f);
    glBindVertexArray(0);
}