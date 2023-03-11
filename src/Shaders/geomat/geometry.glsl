#version 450 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 color;

void build_house(vec4 position);

void main() {
    build_house(gl_in[0].gl_Position);
}

// ---------------- Implementations --------------------------

void build_house(vec4 position) {
    color = gs_in[0].color;
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    color = vec3(1.0, 1.0, 1.0);
    gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}
