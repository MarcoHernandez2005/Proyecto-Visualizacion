#version 330 core
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

uniform float minY;
uniform float maxY;
uniform int useHeightColor;

void main() {
    if (useHeightColor == 1) {
        float t = clamp((FragPos.y - minY) / (maxY - minY), 0.0, 1.0);
        vec3 baseColor = (t < 0.5) ? mix(vec3(0,0,1), vec3(1,1,0), t*2.0) : mix(vec3(1,1,0), vec3(1,0,0), (t-0.5)*2.0);
        
        vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
        vec3 n = normalize(Normal);
        float diff = max(dot(n, lightDir), 0.0);
        FragColor = vec4(baseColor * (0.3 + 0.7 * diff), 1.0);
    } else {
        // Para los ejes (usan el color que viene del VBO)
        FragColor = vec4(Normal, 1.0); // Reutilizamos el atributo 1 como color para los ejes
    }
}