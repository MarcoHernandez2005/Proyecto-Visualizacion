#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float minY;
uniform float maxY;

out float heightFactor; // Factor normalizado [0, 1] de la altura

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Normalizar la altura Y entre 0.0 y 1.0 según la caja AABB
    heightFactor = (aPos.y - minY) / (maxY - minY);
}