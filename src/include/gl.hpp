#pragma once
#include <SDL2/SDL_stdinc.h>
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <assimp/scene.h>
#include <things.hpp>
#include <utility>
#include <memory>
#include <functional>

namespace GL {
    using namespace glm;

    void preset();

    void update_preset();

    class Shader {
        Uint32 program;

        void logShader(Uint32 shader, const char* type);

        void logProgram();
    public:
        Shader(std::string vertexFile, std::string fragmentFile, std::string geometryFile = "");

        Shader(const Shader& other) = delete;

        Shader(Shader&& other) noexcept;

        Shader& operator=(const Shader& other) = delete;

        Shader& operator=(Shader&& other) noexcept;

        void use() const;

        int getUniformLocation(const char* name);

        void setBool(const std::string&& name, bool value) const;

        void setInt(const std::string&& name, int value) const;

        void setFloat(const std::string&& name, float value) const;

        void setVec2(const std::string&& name, const vec2& value) const;

        void setVec3(const std::string&& name, const vec3& value) const;

        void setVec4(const std::string&& name, const vec4& value) const;

        void setMat2(const std::string&& name, const mat2& value) const;

        void setMat3(const std::string&& name, const mat3& value) const;

        void setMat4(const std::string&& name, const mat4& value) const;

        void setMVP(const mat4& model, const mat4& view, const mat4& projection) const;

        void setMV(const mat4& model, const mat4& view) const;

        void setVP(const mat4& view, const mat4& projection) const;

        friend void swap(Shader&, Shader&) noexcept;

        ~Shader();
    };

    enum CameraMovement {
        FORWARD, BACKWARD, LEFT, RIGHT
    };

    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 0.1f;
    const float SENSITIVITY = 0.1f;

    class Camera {
    public:
        vec3 position;
        vec3 front;
        vec3 up;
        vec3 right;
        vec3 worldup;

        // euler angles;
        float yaw;
        float pitch;

        //camera options
        float movement_speed;
        float mouse_sensitivity;

        Camera(vec3 pos = {0.0f, 0.0f, 0.0f}, vec3 u = {0.0f, 1.0f, 0.0f}, float y = YAW, float p = PITCH);

        mat4 GetViewMatrix();

        void ProcessKeyboard(CameraMovement direction, float deltaTime);

        void ProcessMouseMovement(vec2);
    private:
        void updateCameraVectors();
    };

    uint32_t TextureFromFile(const char* path, const std::string& directory);

    Texture LoadTexture(const char* path, std::string type = "texture_diffuse", bool flip = true);

    template<typename VertexType>
    uint32_t create_vbo(std::vector<VertexType>& vertices, int& location) {
        uint32_t vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType), &vertices[0], GL_STATIC_DRAW);

        std::vector<uint8_t> structure = VertexType::get_structure();
        int offset = 0;
        for (const uint8_t& size : structure) {
            glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void*)(offset * sizeof(float)));
            glEnableVertexAttribArray(location);
            offset += size;
            ++location;
        }
        return vbo;
    }

    uint32_t create_ebo(std::vector<uint32_t>& indices);

    uint32_t create_vao();

    uint32_t create_ubo(uint32_t byte_size);

    void set_uniform_buffer(uint32_t ubo, uint32_t offset_byte, uint32_t byte_size, float* data);

    void bind_vbo(const uint32_t& vbo);

    void bind_ebo(const uint32_t& vbo);

    void bind_vao(const uint32_t& vao);
}

namespace gl {

struct vertex {
    glm::vec3 position;
};

}
