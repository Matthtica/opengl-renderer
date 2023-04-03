#pragma once
#include <gl.hpp>
#include <mesh.hpp>

class MeshNode {
public:
    GL::Shader* shader;
    Mesh mesh;

    MeshNode() {};

    MeshNode(const MeshNode&);

    MeshNode(MeshNode&&) noexcept;


};
