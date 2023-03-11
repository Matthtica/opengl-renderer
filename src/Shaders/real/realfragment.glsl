#version 450 core

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_height1;

in vec2 TexCoord;
out vec4 FragColor;

void main() {
    FragColor = vec4(texture(texture_diffuse1, TexCoord).rgb, 1.0);
}
