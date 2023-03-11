#include <mesh.hpp>

Mesh::Mesh(Mesh&& rhs) noexcept {
    VBO = std::exchange(rhs.VBO, 0);
    VAO = std::exchange(rhs.VAO, 0);
    EBO = std::exchange(rhs.EBO, 0);
    vertices_size = std::exchange(rhs.vertices_size, 0);
    indices_size = std::exchange(rhs.indices_size, 0);
    textures = std::move(rhs.textures);
}

Mesh& Mesh::operator=(Mesh&& rhs) noexcept {
    Mesh tmp(std::move(rhs));
    swap(tmp, *this);
    return *this;
}

void Mesh::draw(GL::Shader& shader) const {
    uint32_t diffuseNr = 0;
    uint32_t specularNr = 0;
    uint32_t normalNr = 0;
    uint32_t heightNr = 0;
    for (uint32_t i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(++diffuseNr);
        else if (name == "texture_specular")
            number = std::to_string(++specularNr);
        else if (name == "texture_normal")
            number = std::to_string(++normalNr);
        else if (name == "texture_height")
            number = std::to_string(++heightNr);

        shader.setInt(name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    // draw mesh
    glBindVertexArray(VAO);
    // WARN: async drawing is not working
    glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::drawPoints(GL::Shader& shader) {
    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, vertices_size);
    glBindVertexArray(0);
    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void swap(Mesh& lhs, Mesh& rhs) noexcept {
    std::swap(lhs.VBO, rhs.VBO);
    std::swap(lhs.EBO, rhs.EBO);
    std::swap(lhs.VAO, rhs.VAO);
    std::swap(lhs.vertices_size, rhs.vertices_size);
    std::swap(lhs.indices_size, rhs.indices_size);
    std::swap(lhs.textures, rhs.textures);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
}
