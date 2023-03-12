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
