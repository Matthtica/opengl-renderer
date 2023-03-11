#version 450 core

in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D texture_diffuse1;

void main() {
    FragColor = vec4(TexCoord.xxx, 1.0f);
}
