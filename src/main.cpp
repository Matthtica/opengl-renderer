#include <gl.hpp>
#include <app.cpp>
#include <Application.hpp>
#include <renderer.hpp>

using namespace std;

static int WIDTH = 1280;
static int HEIGHT = 720;

int main() {
    Application& app = Application::get("Application", WIDTH, HEIGHT);
    Renderer& renderer = app.renderer;

    GL::Shader shader("../src/Shaders/real/vertex.glsl", "../src/Shaders/real/fragment.glsl");
    GL::Shader simpleShader("../src/Shaders/simple/vertex.vert", "../src/Shaders/simple/fragment.frag");

    //auto plane = CreatePlane(simpleShader);
    auto cube = CreateCube(simpleShader);

    //renderer << plane.node;
    //renderer << cube.node;
    auto guitor = CreateGuitor(shader, app.camera);
    app.renderer << guitor.node;
    app.run();
    return 0;
}
