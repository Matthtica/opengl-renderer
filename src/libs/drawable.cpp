#include "model.hpp"
#include <drawable.hpp>
#include <utility>

Node::Node(const Node& other) :
    shader(other.shader)
{}

Node::Node(Node&& other) noexcept :
    shader(std::exchange(other.shader, nullptr))
{}

Node& Node::operator=(const Node& other) {
    shader = other.shader;
    return *this;
}

Node& Node::operator=(Node&& other) noexcept {
    shader = std::exchange(other.shader, nullptr);
    return *this;
}

void swap(Node& a, Node& b) noexcept {
    std::swap(a.shader, b.shader);
}

MeshNode::MeshNode(MeshNode&& other) noexcept :
    Node(std::move(other)),
    mesh(std::move(other.mesh))
{}

MeshNode& MeshNode::operator=(MeshNode&& other) noexcept {
    MeshNode tmp(std::move(other));
    swap(tmp, *this);
    return *this;
}

void MeshNode::draw() const {
    shader->use();
    mesh.draw(*shader);
}

void swap(MeshNode& a, MeshNode& b) noexcept {
    std::swap(a.shader, b.shader);
    std::swap(a.mesh, b.mesh);
}

ModelNode::ModelNode(ModelNode&& other) noexcept : 
    Node(std::move(other)), model(std::move(other.model))
{}

ModelNode& ModelNode::operator=(ModelNode&& other) noexcept {
    Node::operator=(std::move(other));
    model = std::move(other.model);
    return *this;
}

void ModelNode::loadModel(std::string path) {
    model.loadModel(path);
}

void ModelNode::draw() const {
    shader->use();
    model.draw(*shader);
}

void swap(ModelNode& a, ModelNode& b) noexcept {
    swap(a.model, b.model);
}
