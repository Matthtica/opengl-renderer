#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <glm/glm.hpp>
#include <string>
#include <gl.hpp>
#include <vector>
#include <drawable.hpp>
#include <glm/gtc/matrix_transform.hpp>

// TODO: create utility library to easily create procedural mesh with sexy code.
auto CreatePlane(GL::Shader& shader) {
    struct ResultObject {
        MeshObject object;
        glm::mat4 model = glm::mat4(1.0f);
        float scaler = 1.0f;
    } result;
    result.object.shader = &shader;
    struct Vertex {
        glm::vec2 position;
        glm::vec2 tex;

        Vertex(glm::vec2 pos, glm::vec2 tex): position(pos), tex(tex) {}

        static constexpr std::vector<uint8_t> get_structure() {
            return {2, 2};
        }
    };

    std::vector<Vertex> vertices;
    vertices.reserve(4);
    vertices.emplace_back(Vertex({-1.0f, 1.0f}, {0.0f, 1.0f}));
    vertices.emplace_back(Vertex({1.0f, 1.0f}, {1.0f, 1.0f}));
    vertices.emplace_back(Vertex({-1.0f, -1.0f}, {0.0f, 0.0f}));
    vertices.emplace_back(Vertex({1.0f, -1.0f}, {1.0f, 0.0f}));

    std::vector<uint32_t> indices = {
        2, 1, 0,
        3, 1, 2
    };

    int location = 0;
    uint32_t len = indices.size();

    result.object.mesh.setup(vertices, indices);

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 1.0f));

    result.object.setupUniformUpdator([&result](const GL::Shader& shader) {
        glm::mat4 newmodel = scale(result.model, glm::vec3(result.scaler));
        shader.setMat4("model", newmodel);
    });
    result.object.setupImgui([&result](){
        ImGui::Begin("Scaler");
        ImGui::SliderFloat("scale", &result.scaler, 0.0, 2.0);
        ImGui::End();
    });
    return result;
}

auto CreateGuitor(GL::Shader& shader) {
    struct Result {
        ModelObject object;
        float scale = 1.0;
    } result;
    result.object.shader = &shader;
    result.object.loadModel("../assets/backpack/backpack.obj");

    result.object.setupUniformUpdator([&result](const GL::Shader& shader){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(result.scale));
        shader.setMat4("model", model);
    });

    result.object.setupImgui([&result]() {
        ImGui::Begin("Guitor");
        ImGui::NewLine();
        ImGui::SliderFloat("scale", &result.scale, 0.0f, 2.0f);
        ImGui::End();
    });
    return result;
}
