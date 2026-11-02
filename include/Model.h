#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class ShaderProgram;

class Model {
public:
    Model(ShaderProgram* shader, const std::string& objPath);
    // NUEVO CONSTRUCTOR PARA EL PLANO MATEMÁTICO:
    Model(ShaderProgram* shader, const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices, glm::vec3 minAABB, glm::vec3 maxAABB);
    ~Model();
    
    void renderModel(const glm::mat4& view, const glm::mat4& projection);
    void renderAxis(const glm::mat4& view, const glm::mat4& projection);
    
    float rotX, rotY, rotZ;
    float scaleMultiplier;
    int drawMode;

    glm::vec3 getMinAABB() const { return m_minAABB; }
    glm::vec3 getMaxAABB() const { return m_maxAABB; }

private:
    GLuint m_VAO, m_VBO, m_EBO;
    GLuint m_axisVAO, m_axisVBO;
    int m_indexCount;
    glm::vec3 m_minAABB, m_maxAABB, m_center;
    ShaderProgram* m_shader;

    void loadOBJ(const std::string& path);
    void setupBuffers(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices);
};