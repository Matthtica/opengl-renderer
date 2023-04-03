#include "traits.hpp"
#include <model.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <fgl.hpp>
#include <glstd.hpp>

Model::Model(std::string path, bool gamma): gammaCorrection(gamma) {
    loadModel(path);
}

Model::Model(Model&& other) noexcept:
   loaded_textures(std::move(other.loaded_textures)),
   meshes(std::move(other.meshes)),
   directory(std::move(other.directory)),
   gammaCorrection(std::move(other.gammaCorrection))
{}

Model& Model::operator=(Model&& other) noexcept {
    Model tmp(std::move(other));
    swap(*this, tmp);
    return *this;
}

void Model::loadModel(std::string path) {
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate           |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType           |
            aiProcess_FlipUVs               |
            aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.emplace_back(processMesh(mesh, scene));
    }

    for (uint32_t i = 0; i < node->mNumChildren; ++i)
        processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    using namespace glm;
    using namespace fgl::traits;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        if (mesh->HasNormals())
            vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            vertex.Tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
            vertex.Bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
        } else vertex.TexCoords = {0.0f, 0.0f};
        vertices.push_back(vertex);
    }
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(glstd::flatten(vertices), indices, layout_of<vec3, vec3, vec2, vec3, vec3>(), textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for (uint32_t i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (uint32_t j = 0; j < loaded_textures.size(); ++j) {
            if (std::strcmp(loaded_textures[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(loaded_textures[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id = GL::TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            loaded_textures.push_back(texture);
        }
    }
    return textures;
}

void Model::draw(GL::Shader& shader) const {
    for (uint32_t i = 0; i < meshes.size(); ++i) {
        meshes[i].draw(shader);
    }
}

void swap(Model& a, Model& b) noexcept {
    std::swap(a.loaded_textures, b.loaded_textures);
    std::swap(a.meshes, b.meshes);
    std::swap(a.directory, b.directory);
    std::swap(a.gammaCorrection, b.gammaCorrection);
}
