#version 450 core

in vec2 TexCoord;

uniform sampler2D texture_diffuse1;

out vec4 FragColor;

void main() {
    FragColor = texture(texture_diffuse1, TexCoord);
}
