#version 410 core
in float localY;
out vec4 color;

uniform float minY;
uniform float maxY;

void main()
{
    // Definicion de colores
    vec3 blue = vec3(0.0, 0.0, 1.0);
    vec3 yellow = vec3(1.0, 1.0, 0.0);
    vec3 red = vec3(1.0, 0.0, 0.0);

    // Normalizar la altura    
    float t = (localY - minY) / (maxY - minY);

    vec3 fragColor;

    if (t < 0.5) {
        // Reescalar el rango [0.0, 0.5] a [0.0, 1.0]
        fragColor = mix(blue, yellow, t * 2.0f);
    } else {
        // Reescalar el rango [0.5, 1.0] a [0.0, 1.0]
        fragColor = mix(yellow, red, (t - 0.5f) * 2.0f);
    }

    color = vec4(fragColor, 1.0);
}