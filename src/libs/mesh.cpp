#include <mesh.hpp>
#include <glstd.hpp>
#include <numeric>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& structure, const std::vector<Texture>& textures) {
    this->textures = textures;
    setup(vertices, indices, structure);
}

Mesh::Mesh(Mesh&& rhs) noexcept {
    VBO = std::exchange(rhs.VBO, 0);
    VAO = std::exchange(rhs.VAO, 0);
    EBO = std::exchange(rhs.EBO, 0);
    num_vertex = std::exchange(rhs.num_vertex, 0);
    num_index = std::exchange(rhs.num_index, 0);
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
    glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::drawPoints(GL::Shader& shader) {
    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, num_vertex);
    glBindVertexArray(0);
    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupVertices(const std::vector<float>& vertices, const std::vector<uint32_t>& structure) {
    VAO = GL::create_vao();
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    size_t vertex_size = std::accumulate(structure.begin(), structure.end(), 0);

    num_vertex = vertices.size() * sizeof(float) / vertex_size;
    uint64_t prev = 0;
    for (uint32_t i = 0; i < structure.size(); ++i) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, structure[i] / sizeof(float), GL_FLOAT, GL_FALSE, vertex_size, (void*)prev);
        prev += structure[i];
    }
}

void Mesh::setup(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& structure) {
    setupVertices(vertices, structure);
    num_index = indices.size();
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_index * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
}

void swap(Mesh& lhs, Mesh& rhs) noexcept {
    std::swap(lhs.VBO, rhs.VBO);
    std::swap(lhs.EBO, rhs.EBO);
    std::swap(lhs.VAO, rhs.VAO);
    std::swap(lhs.num_vertex, rhs.num_vertex);
    std::swap(lhs.num_index, rhs.num_index);
    std::swap(lhs.textures, rhs.textures);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
}
