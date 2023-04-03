#pragma once
#include <drawable.hpp>
#include <vector>

// TODO: Complex things are better with scene GRAPH approach
// TODO: Check for object slicing and avoid duplication by virtual
class Renderer {
    std::vector<Node*> objects;
public:
    Renderer() {};

    void operator<<(Node& object);

    void draw() const;

    friend class Application;
};
