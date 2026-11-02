#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

class OrbitCamera {
public:
    OrbitCamera(glm::vec3 pos, glm::vec3 coi, glm::vec3 up);
    glm::mat4 getMatrix();
    void rotate(glm::vec2 mouseInit, glm::vec2 mouseCurr);
    void setPos(glm::vec2 mouseInit, glm::vec2 mouseCurr);

private:
    glm::vec2 getAngles(glm::vec2 mouseInit, glm::vec2 mouseCurr);
    
    glm::vec3 m_pos;
    glm::vec3 m_coi;
    glm::vec3 m_up;
    float m_theta;
    float m_phi;
    float m_radius;
};