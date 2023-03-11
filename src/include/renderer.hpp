#pragma once
#include <drawable.hpp>
#include <vector>

// TODO: Complex things are better with scene GRAPH approach
// TODO: Check for object slicing and avoid duplication by virtual
class Renderer {
    std::vector<Drawable*> objects;
public:
    Renderer() {};

    void add(Drawable& object);

    void draw() const;

    friend class Application;
};
