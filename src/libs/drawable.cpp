#include "model.hpp"
#include <drawable.hpp>
#include <utility>

Drawable::Drawable(const Drawable& other) :
    update_uniform(other.update_uniform),
    draw_imgui(other.draw_imgui),
    updator(other.updator),
    shader(other.shader)
{}

Drawable::Drawable(Drawable&& other) noexcept :
    update_uniform(std::exchange(other.update_uniform, nullptr)),
    draw_imgui(std::exchange(other.draw_imgui, nullptr)),
    updator(std::exchange(other.updator, nullptr)),
    shader(std::exchange(other.shader, nullptr))
{}

Drawable& Drawable::operator=(const Drawable& other) {
    update_uniform = other.update_uniform;
    draw_imgui = other.draw_imgui;
    updator = other.updator;
    shader = other.shader;
    return *this;
}

Drawable& Drawable::operator=(Drawable&& other) noexcept {
    update_uniform = std::exchange(other.update_uniform, nullptr);
    draw_imgui = std::exchange(other.draw_imgui, nullptr);
    updator = std::exchange(other.updator, nullptr);
    return *this;
}

void Drawable::setupUniformUpdator(std::function<void(const GL::Shader&)> fn) {
    update_uniform = fn;
}

void Drawable::setupImgui(std::function<void()> fn) {
    draw_imgui = fn;
}

void Drawable::setupUpdator(std::function<void()> fn) {
    updator = fn;
}

void Drawable::drawImgui() const {
    if (draw_imgui != nullptr) draw_imgui();
}

void Drawable::update() {
    if (updator != nullptr) updator();
}

void swap(Drawable& a, Drawable& b) noexcept {
    std::swap(a.update_uniform, b.update_uniform);
    std::swap(a.draw_imgui, b.draw_imgui);
    std::swap(a.updator, b.updator);
}

Object::Object(Object&& other) noexcept {
    update_uniform = std::exchange(other.update_uniform, nullptr);
    drawer = std::exchange(other.drawer, nullptr);
    draw_imgui = std::exchange(other.draw_imgui, nullptr);
    updator = std::exchange(other.updator, nullptr);
    shader = std::exchange(other.shader, nullptr);
}

Object& Object::operator=(Object &&other) noexcept {
    Object tmp(std::move(other));
    swap(tmp, *this);
    return *this;
}

void Object::setupDrawer(std::function<void ()> fn) {
    drawer = fn;
}

void Object::draw() const {
    shader->use();
    if (update_uniform != nullptr) update_uniform(*shader);
    if (drawer == nullptr)
        printf("Didn't provided drawer of 'Object' class\n");
    else drawer();
}

Object::~Object() {
    glDeleteVertexArrays(1, &vao);
    for (uint32_t& vbo : vbos) glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void swap(Object& a, Object& b) noexcept {
    std::swap(a.shader, b.shader);
    std::swap(a.vao, b.vao);
    std::swap(a.vbos, b.vbos);
    std::swap(a.ebo, b.ebo);
    std::swap(a.update_uniform, b.update_uniform);
    std::swap(a.drawer, b.drawer);
    std::swap(a.draw_imgui, b.draw_imgui);
    std::swap(a.updator, b.updator);
}

MeshObject::MeshObject(MeshObject&& other) noexcept :
    Drawable(std::move(other)),
    mesh(std::move(other.mesh))
{}

MeshObject& MeshObject::operator=(MeshObject&& other) noexcept {
    MeshObject tmp(std::move(other));
    swap(tmp, *this);
    return *this;
}

void MeshObject::draw() const {
    shader->use();
    if (update_uniform != nullptr) update_uniform(*shader);
    mesh.draw(*shader);
}

void swap(MeshObject& a, MeshObject& b) noexcept {
    std::swap(a.shader, b.shader);
    std::swap(a.update_uniform, b.update_uniform);
    std::swap(a.draw_imgui, b.draw_imgui);
    std::swap(a.updator, b.updator);
    std::swap(a.mesh, b.mesh);
}

ModelObject::ModelObject(ModelObject&& other) noexcept : 
    Drawable(std::move(other)), model(std::move(other.model))
{}

ModelObject& ModelObject::operator=(ModelObject&& other) noexcept {
    Drawable::operator=(std::move(other));
    model = std::move(other.model);
    return *this;
}

void ModelObject::loadModel(std::string path) {
    model.loadModel(path);
}

void ModelObject::draw() const {
    shader->use();
    if (update_uniform != nullptr) update_uniform(*shader);
    model.draw(*shader);
}
