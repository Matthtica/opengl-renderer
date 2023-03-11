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
class Drawable {
protected:
    std::function<void(const GL::Shader&)> update_uniform;
    std::function<void()> draw_imgui;
    std::function<void()> updator;
public:
    GL::Shader* shader;

    Drawable() {};

    Drawable(const Drawable&);

    Drawable(Drawable&&) noexcept;

    Drawable& operator=(const Drawable&);

    Drawable& operator=(Drawable&&) noexcept;

    void setupUniformUpdator(std::function<void(const GL::Shader&)> fn);

    void setupImgui(std::function<void()> fn);

    void setupUpdator(std::function<void()> fn);

    virtual void draw() const = 0;

    void drawImgui() const;

    void update();

    friend void swap(Drawable&, Drawable&) noexcept;
};

class Object : public Drawable {
    std::function<void()> drawer;
public:
    uint32_t vao;
    std::vector<uint32_t> vbos;
    uint32_t ebo;

    Object() {};

    Object(const Object& other) = delete;

    Object(Object&& other) noexcept;

    Object& operator=(const Object& other) = delete;

    Object& operator=(Object&& other) noexcept;

    void setupDrawer(std::function<void()> fn);

    void draw() const override;

    friend void swap(Object&, Object&) noexcept;

    ~Object();
};

class MeshObject : public Drawable {
public:
    Mesh mesh;
    MeshObject() {}

    MeshObject(const MeshObject&) = delete;

    MeshObject(MeshObject&& other) noexcept;

    MeshObject& operator=(const MeshObject&) = delete;

    MeshObject& operator=(MeshObject&& other) noexcept;

    void draw() const override;

    friend void swap(MeshObject&, MeshObject&) noexcept;
};

class ModelObject : public Drawable {
public:
    Model model;
    ModelObject() {}

    ModelObject(std::string path): model(path) {};

    ModelObject(const ModelObject&) = delete;

    ModelObject(ModelObject&& other) noexcept;

    ModelObject& operator=(const ModelObject&) = delete;

    ModelObject& operator=(ModelObject&& other) noexcept;

    void draw() const override;

    void loadModel(std::string path);
};
