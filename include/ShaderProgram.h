#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class ShaderProgram {
public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    void use() const { glUseProgram(m_id); }
    
    void setMat4x4(const std::string& name, const glm::mat4& value) const;
    void setFloat(const std::string& name, float value) const;
    void setInt(const std::string& name, int value) const;

private:
    GLuint m_id;
    std::string readShaderSource(const char* filePath);
    GLuint compileShader(GLenum type, const std::string& src);
    void shaderLog(GLuint shader_id) const;
    void programLog() const;
};