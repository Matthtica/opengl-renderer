#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <glm/glm.hpp>
#include <string>
#include <gl.hpp>
#include <vector>
#include <drawable.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glstd.hpp>
#include <ranges>

// TODO: create utility library to easily create procedural mesh with sexy code.
auto CreatePlane(GL::Shader& shader) {
    struct ResultObject {
        MeshNode node;
        glm::mat4 model = glm::mat4(1.0f);
        float scaler = 1.0f;
    } result;
    result.node.shader = &shader;
    struct Vertex {
        glm::vec3 position;
        glm::vec2 tex;

        Vertex(glm::vec3 pos, glm::vec2 tex): position(pos), tex(tex) {}

        static constexpr std::vector<uint8_t> get_structure() {
            return {3, 2};
        }
    };

    std::vector<Vertex> vertices;
    vertices.reserve(4);
    vertices.emplace_back(Vertex({-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}));
    vertices.emplace_back(Vertex({1.0f, -1.0f, -1.0f}, {1.0f, 1.0f}));
    vertices.emplace_back(Vertex({-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f}));
    vertices.emplace_back(Vertex({1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}));

    std::vector<uint32_t> indices = {
        2, 1, 0,
        3, 1, 2
    };

    int location = 0;
    uint32_t len = indices.size();

    result.node.mesh.setup(vertices, indices);
    Texture texture = GL::LoadTexture("../assets/wood-fade.jpg");
    result.node.mesh.textures = {texture};

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));
    shader.setMat4("model", model);

    return result;
}

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

auto CreateCube(GL::Shader& shader) {
    struct Resurt {
        MeshNode node;
    } res;

    struct Vertex {
        glm::vec3 position;

        static constexpr std::vector<uint8_t> get_structure() {
            return {3};
        }
    };
    std::vector<Vertex> vertices;
    vertices.emplace_back(Vertex{{-1.0, 1.0, 1.0}});
    vertices.emplace_back(Vertex{{1.0, 1.0, 1.0}});
    vertices.emplace_back(Vertex{{-1.0,-1.0, 1.0}});
    vertices.emplace_back(Vertex{{1.0,-1.0, 1.0}});

    vertices.emplace_back(Vertex{{-1.0, 1.0, -1.0}});
    vertices.emplace_back(Vertex{{1.0, 1.0, -1.0}});
    vertices.emplace_back(Vertex{{-1.0,-1.0, -1.0}});
    vertices.emplace_back(Vertex{{1.0,-1.0, -1.0}});

    std::vector<uint32_t> indices = cube_index();

    res.node.shader = &shader;
    res.node.mesh.setup(vertices, indices);
    shader.use();
    shader.setMat4("model", glm::mat4(1.0f));
    return res;
}

auto CreateGuitor(GL::Shader& shader, GL::Camera& camera) {
    struct Result {
        ModelNode node;
        float scale = 1.0;
    } result;
    result.node.shader = &shader;
    result.node.loadModel("../assets/backpack/backpack.obj");
    shader.use();
    shader.setMat4("model", glm::mat4(1.0f));

    return result;
}
