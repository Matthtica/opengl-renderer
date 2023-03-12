#include <gl.hpp>
#include <glad/glad.h>
#include <app.cpp>
#include <Application.hpp>

using namespace std;

static int WIDTH = 1280;
static int HEIGHT = 720;

int main() {
    Application& application = Application::get("Application", WIDTH, HEIGHT);

    GL::Shader shader("../src/Shaders/real/vertex.glsl", "../src/Shaders/real/fragment.glsl");
    GL::Shader simpleShader("../src/Shaders/simple/vertex.glsl", "../src/Shaders/simple/fragment.glsl");

    auto plane = CreatePlane(simpleShader);
    application.renderer.add(plane.object);

    //auto guitor = CreateGuitor(shader);
    //application.renderer.add(guitor.object);

    /*
    ModelObject object;
    object.shader = &shader;
    glm::vec3 trans = {0.0, 0.0, 0.0};
    std::thread loader([&object, &application, &trans]() {
        object.loadModel("../src/resources/backpack/backpack.obj");
        object.setupUniformUpdator([&object, &trans](const GL::Shader& shader) {
            glm::mat4 model = glm::mat4(1.0f);
            model = translate(model, trans);
            shader.setMat4("model", model);
        });
        object.setupImgui([&trans]() {
            ImGui::Begin("Guitor");
            ImGui::NewLine();
            ImGui::ColorEdit3("color", (float*)&trans);
            ImGui::End();
        });
        application.data_mut.lock();
        application.renderer.add(object);
        application.data_mut.unlock();
    });*/
    application.run();
    return 0;
}
