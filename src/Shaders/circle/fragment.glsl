#version 450 core

out vec4 FragColor;
in vec3 pos;

void main() {
    float len = length(pos);
    if (len < 0.5)
        discard;
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
