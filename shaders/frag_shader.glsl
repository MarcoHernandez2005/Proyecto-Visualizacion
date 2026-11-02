#version 330 core
in float heightFactor;
out vec4 FragColor;

void main() {
    // Definir colores del mapa: Azul (bajo), Amarillo (medio), Rojo (alto)[cite: 4]
    vec3 blue   = vec3(0.0, 0.0, 1.0);
    vec3 yellow = vec3(1.0, 1.0, 0.0);
    vec3 red    = vec3(1.0, 0.0, 0.0);

    vec3 color;
    if (heightFactor < 0.5) {
        // Interpola entre Azul y Amarillo para la mitad inferior
        color = mix(blue, yellow, heightFactor * 2.0);
    } else {
        // Interpola entre Amarillo y Rojo para la mitad superior
        color = mix(yellow, red, (heightFactor - 0.5) * 2.0);
    }

    FragColor = vec4(color, 1.0);
}