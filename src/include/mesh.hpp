#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <glad/glad.h>
#include <gl.hpp>
#include <things.hpp>
#include <utility>
#include <type_traits>
#include <iostream>

#define MAX_BONE_INFLUENCE 4

template<typename T>
concept StructuredVertexType = requires {
    { T::get_structure() } -> std::convertible_to<std::vector<uint8_t>>;
};

class Mesh {
private:
    uint32_t VBO, EBO, VAO;
    size_t vertices_size;
    size_t indices_size;
public:
    std::vector<Texture> textures;

    Mesh() {}

    template<typename VertexType> requires StructuredVertexType<VertexType>
    Mesh(std::vector<VertexType> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures) {
        this->textures = textures;
        setup(vertices, indices);
    }

    friend void swap(Mesh& lhs, Mesh& rhs) noexcept;

    Mesh(const Mesh& rhs) = delete;

    Mesh(Mesh&& rhs) noexcept;

    Mesh& operator=(const Mesh&) = delete;

    Mesh& operator=(Mesh&& rhs) noexcept;

    void draw(GL::Shader& shader) const;

    void drawPoints(GL::Shader& shader);

    template<typename VertexType> requires StructuredVertexType<VertexType>
    void setupVertices(std::vector<VertexType>& vertices) {
        VAO = GL::create_vao();
        vertices_size = vertices.size();
        std::vector<uint8_t> structure = VertexType::get_structure();
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(VertexType), &vertices[0], GL_STATIC_DRAW);

        for (int i = 0, prev = 0; i < structure.size(); ++i) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, structure[i], GL_FLOAT, GL_FALSE, sizeof(VertexType), (void*)(prev * sizeof(float)));
            prev += structure[i];
        }
    }

    template<typename VertexType> requires StructuredVertexType<VertexType>
    void setup(std::vector<VertexType>& vertices, std::vector<uint32_t>& indices) {
        setupVertices(vertices);

        indices_size = indices.size();
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
    }

    ~Mesh();
};
