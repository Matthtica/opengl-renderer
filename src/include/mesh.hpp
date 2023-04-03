#pragma once
#include <vector>
#include <gl.hpp>

#define MAX_BONE_INFLUENCE 4

class Mesh {
private:
    uint32_t VBO, EBO, VAO;
    size_t num_vertex;
    size_t num_index;
public:
    std::vector<Texture> textures;

    Mesh() {}

    Mesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& structure, const std::vector<Texture>& textures);

    friend void swap(Mesh& lhs, Mesh& rhs) noexcept;

    Mesh(const Mesh& rhs) = delete;

    Mesh(Mesh&& rhs) noexcept;

    Mesh& operator=(const Mesh&) = delete;

    Mesh& operator=(Mesh&& rhs) noexcept;

    void draw(GL::Shader& shader) const;

    void drawPoints(GL::Shader& shader);

    void setupVertices(const std::vector<float>& vertices, const std::vector<uint32_t>& structure);

    void setup(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& structure);

    ~Mesh();
};
