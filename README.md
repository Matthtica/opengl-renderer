# OpenDraw

## Goal

The goal of this OpenDraw library is to abstract away the boilerplate that
OpenGL brings and provide a library with irreducible complexity.

## Usage

Create, setup uniform update, and imgui at initialization in a single function.

```cpp
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
```

Rendering that object.

```cpp
#include <gl.hpp>
#include <app.cpp>
#include <Application.hpp>

using namespace std;

static int WIDTH = 1280;
static int HEIGHT = 720;

int main() {
    Application& application = Application::get("Application", WIDTH, HEIGHT);

    GL::Shader shader("../src/Shaders/real/vertex.glsl", "../src/Shaders/real/fragment.glsl");

    auto guitor = CreateGuitor(shader);
    application.renderer.add(guitor.object);

    application.run();
    return 0;
}
```

Result
