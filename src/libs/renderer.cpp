#include <renderer.hpp>

void Renderer::add(Drawable& object) {
    objects.emplace_back(&object);
}

void Renderer::draw() const {
    for(const Drawable* object : objects) object->draw();
}
