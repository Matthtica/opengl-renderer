#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <assimp/scene.h>
#include <gl.hpp>
#include <mesh.hpp>

class Model {
private:
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;

        static constexpr std::vector<uint8_t> get_structure() {
            return {3, 3, 2, 3, 3};
        }
    };
    std::vector<Texture> loaded_textures;
    std::vector<Mesh> meshes;

    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
    std::string directory;
    bool gammaCorrection;

    Model(): gammaCorrection(false) {}

    Model(const Model&) = delete;

    Model(Model&& other) noexcept;

    Model& operator=(const Model&) = delete;

    Model& operator=(Model&& other) noexcept;

    Model(std::string path, bool gamma = false);

    void loadModel(std::string path);

    void draw(GL::Shader& shader) const;

    friend void swap(Model& a, Model& b) noexcept;
};
