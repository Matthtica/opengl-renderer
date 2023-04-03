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

using Indices = std::vector<uint32_t>;

template<typename VertexType>
using Vertices = std::vector<VertexType>;

class Index {
    std::vector<uint32_t> cont;
public:
    constexpr Index() {}

    constexpr Index(const std::initializer_list<uint32_t>& li) {
        cont.insert(cont.end(), li.begin(), li.end());
    }

    constexpr Index(const std::vector<uint32_t>& in): cont(in) {}

    constexpr Index(const Index& a, const Index& b) {
        cont.reserve(a.cont.size() + b.cont.size());
        cont.insert(cont.end(), a.cont.begin(), a.cont.end());
        cont.insert(cont.end(), b.cont.begin(), b.cont.end());
    }

    constexpr Index operator+(const Index& rhs) {
        Index res(*this, rhs);
        return res;
    }

    operator std::vector<uint32_t>() {
        return cont;
    }
};

constexpr Index plane_index(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    return {a, c, b, c, d, b};
}

constexpr Index cube_index() {
    return plane_index(0, 1, 2, 3) + plane_index(5, 4, 7, 6) +
        plane_index(1, 5, 3, 7) + plane_index(4, 0, 6, 2) +
        plane_index(4, 5, 0, 1) + plane_index(2, 3, 6, 7);
}

template<typename T>
class verd {
    T* cont;
    size_t size;
public:
    constexpr verd(): cont(nullptr), size(0) {}

    verd(const std::initializer_list<T>& in) {
        size = in.size();
        cont = new T(size);
        auto it = in.begin();
        for (size_t i = 0; i < size; ++i, ++it)
            cont[i] = *it;
    }

    template<size_t n>
    constexpr verd(const glm::vec<n, T, glm::defaultp>& in) {
        size = n;
        cont = new T(size);
        memcpy(cont, in, size * sizeof(T));
    }

    constexpr verd(const verd& another) {
        cont = new T(another.size);
        size = another.size;
        memcpy(cont, another.cont, size * sizeof(T));
    }

    constexpr verd(verd&& another) noexcept {
        cont = another.cont;
        size = another.size;
        another.cont = nullptr;
        another.size = 0;
    }

    constexpr verd& operator=(const verd& another) {
        if (size != another.size) {
            delete [] cont;
            size = another.size;
            cont = new T(size);
        }
        memcpy(cont, another.cont, size * sizeof(T));
        return *this;
    }

    constexpr verd& operator=(verd&& another) noexcept {
        verd<T> tmp(std::move(another));
        swap(*this, tmp);
        return *this;
    }

    constexpr verd operator+(const verd& another) {
        verd res;
        size_t n = size + another.size;
        res.cont = new T(n);
        memcpy(res.cont, cont, size * sizeof(T));
        memcpy(res.cont + size, another.cont, another.size * sizeof(T));
        return res;
    }

    constexpr std::vector<T> data() const {
        return std::vector<T>(cont, cont + size);
    }

    template<typename U>
    constexpr friend void swap(verd<U>& a, verd<U>& b) noexcept;

    constexpr ~verd() {
        delete [] cont;
        cont = nullptr;
        size = 0;
    }
};

template<typename U>
constexpr void swap(verd<U>& a, verd<U>& b) noexcept {
    std::swap(a.cont, b.cont);
    std::swap(a.size, b.size);
}

using v = verd<float>;

}
