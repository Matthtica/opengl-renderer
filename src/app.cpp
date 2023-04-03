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
#include <traits.hpp>

// TODO: create utility library to easily create procedural mesh with sexy code.
auto CreatePlane(GL::Shader& shader) {
    using namespace glm;
    using namespace glstd;
    using namespace fgl::traits;

    struct ResultObject {
        MeshNode node;
        mat4 model = mat4(1.0f);
        float scaler = 1.0f;
    } result;
    result.node.shader = &shader;

    struct Vertex {
        vec3 position;
        vec2 tex;

        Vertex(vec3 pos, vec2 tex): position(pos), tex(tex) {}
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

    result.node.setup(flatten(vertices), indices, layout_of<vec3, vec2>());
    Texture texture = GL::LoadTexture("../assets/wood-fade.jpg");
    result.node.mesh.textures = {texture};

    mat4 model = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
    shader.setMat4("model", model);

    return result;
}



auto CreateCube(GL::Shader& shader) {
    using namespace glm;
    using namespace gl;
    using namespace glstd;
    using namespace fgl::traits;

    struct Resurt {
        MeshNode node;
    } res;

    std::vector<vertex> vertices;
    vertices.emplace_back(vertex{{-1.0, 1.0, 1.0}});
    vertices.emplace_back(vertex{{1.0, 1.0, 1.0}});
    vertices.emplace_back(vertex{{-1.0,-1.0, 1.0}});
    vertices.emplace_back(vertex{{1.0,-1.0, 1.0}});

    vertices.emplace_back(vertex{{-1.0, 1.0, -1.0}});
    vertices.emplace_back(vertex{{1.0, 1.0, -1.0}});
    vertices.emplace_back(vertex{{-1.0,-1.0, -1.0}});
    vertices.emplace_back(vertex{{1.0,-1.0, -1.0}});

    std::vector<uint32_t> indices = cube_index();

    res.node.shader = &shader;
    res.node.setup(flatten(vertices), indices, layout_of<vec3>());
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
