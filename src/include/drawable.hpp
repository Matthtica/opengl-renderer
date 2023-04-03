#pragma once
#include <gl.hpp>
#include <functional>
#include <vector>
#include <cstdint>
#include <model.hpp>

// TODO: Recheck moving, copying, constructing, move assigning.
// TODO: Consider adding object's position, orientation and scale to Drawable class
// TODO: via transformation matrix
// This would allow you to easily transform the object before drawing it.
// TODO: Create renderer class in different file.
// TODO: Create Scene graph data structure.
// TODO: Renderer will traverse the scene graph and call draw
// TODO: Require scene graph and different type of scene node.
// TODO: Material class
// TODO: Need a class to draw only imgui without 3d object to work flexibly
class Node {
protected:
public:
    GL::Shader* shader;

    Node() {};

    Node(const Node&);

    Node(Node&&) noexcept;

    Node& operator=(const Node&);

    Node& operator=(Node&&) noexcept;

    virtual void draw() const = 0;

    friend void swap(Node&, Node&) noexcept;
};

class MeshNode : public Node {
public:
    Mesh mesh;
    MeshNode() {}

    MeshNode(const MeshNode&) = delete;

    MeshNode(MeshNode&& other) noexcept;

    MeshNode& operator=(const MeshNode&) = delete;

    MeshNode& operator=(MeshNode&& other) noexcept;

    void setup(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& structure);

    void draw() const override;

    friend void swap(MeshNode&, MeshNode&) noexcept;
};

class ModelNode : public Node {
public:
    Model model;
    ModelNode() {}

    ModelNode(std::string path): model(path) {};

    ModelNode(const ModelNode&) = delete;

    ModelNode(ModelNode&& other) noexcept;

    ModelNode& operator=(const ModelNode&) = delete;

    ModelNode& operator=(ModelNode&& other) noexcept;

    void draw() const override;

    void loadModel(std::string path);

    friend void swap(ModelNode& a, ModelNode& b) noexcept;
};
