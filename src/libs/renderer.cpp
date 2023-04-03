#include <renderer.hpp>

void Renderer::operator<<(Node& object) {
    objects.emplace_back(&object);
}

void Renderer::draw() const {
    for(const Node* object : objects) object->draw();
}
