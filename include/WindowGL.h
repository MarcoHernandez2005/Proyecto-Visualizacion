#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class WindowGL {
public:
    WindowGL() {
        InitGLFW();
        InitGLEW();
    }
    ~WindowGL() { glfwTerminate(); }
    GLFWwindow* getWindow() { return m_window; }
    float getAspectRatio() { return (float)m_width / (float)m_height; }

private:
    GLFWwindow* m_window;
    int m_width = 1024;
    int m_height = 768;
    int m_glfwMajor = 3;
    int m_glfwMinor = 3;
    const char* m_name = "Proyecto Medio Semestre";

    void InitGLFW();
    void InitGLEW();
};