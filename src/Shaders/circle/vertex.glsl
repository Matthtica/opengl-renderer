#version 450 core

layout (location = 0) in vec3 aPos;

layout (std140, binding = 0) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
out vec3 pos;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    pos = aPos;
}
