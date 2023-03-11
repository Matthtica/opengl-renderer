#version 450 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform float mag;

void GenerateLine(int index, vec3 normal);
vec3 GetNormal();

void main() {
    vec3 normal = GetNormal();
    GenerateLine(0, normal);
    GenerateLine(1, normal);
    GenerateLine(2, normal);
}

vec3 GetNormal() {
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void GenerateLine(int index, vec3 normal) {
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = (gl_in[index].gl_Position + vec4(normal, 0.0) * mag);
    EmitVertex();
    EndPrimitive();
}
