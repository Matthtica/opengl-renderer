#include <lights.hpp>
#include <imgui.h>

void DirectionalLight::Imgui(std::string&& title) {
    ImGui::Text("%s", title.c_str());
    ImGui::Text("%s", "direction");
    ImGui::SliderFloat((title + ".direction: x").c_str(), &direction.x, -1.0f, 1.0f);
    ImGui::SliderFloat((title + ".direction: y").c_str(), &direction.y, -1.0f, 1.0f);
    ImGui::SliderFloat((title + ".direction: z").c_str(), &direction.z, -1.0f, 1.0f);
    ImGui::ColorEdit3((title + ".ambient").c_str(), &ambient[0]);
    ImGui::ColorEdit3((title + ".diffuse").c_str(), &diffuse[0]);
    ImGui::ColorEdit3((title + ".specular").c_str(), &specular[0]);
}

void DirectionalLight::setTo(GL::Shader& shade, std::string name) {
    shade.use();
    shade.setVec3(name + ".direction", direction);
    shade.setVec3(name + ".ambient", ambient);
    shade.setVec3(name + ".diffuse", diffuse);
    shade.setVec3(name + ".specular", specular);
}

void PointLight::Imgui(std::string&& title) {
    ImGui::Text("%s", title.c_str());
    ImGui::SliderFloat((title + ".position: x").c_str(), &position.x, -20.0f, 20.0f);
    ImGui::SliderFloat((title + ".position: y").c_str(), &position.y, -20.0f, 20.0f);
    ImGui::SliderFloat((title + ".position: z").c_str(), &position.z, -20.0f, 20.0f);

    ImGui::Text("%s", "ambient");
    ImGui::ColorEdit4((title + ".ambient").c_str(), &ambient[0]);
    ImGui::ColorEdit4((title + ".diffuse").c_str(), &diffuse[0]);
    ImGui::ColorEdit4((title + ".specular").c_str(), &specular[0]);

    ImGui::Text("%s", "fade");
    ImGui::SliderFloat((title + ".constant").c_str(), &constant, 0.0f, 1.0f);
    ImGui::SliderFloat((title + ".linear").c_str(), &linear, 0.0f, 1.0f);
    ImGui::SliderFloat((title + ".quadratic").c_str(), &quadratic, 0.0f, 0.5f);
}

void PointLight::setTo(GL::Shader& shade, std::string name) const {
    shade.use();
    shade.setVec3(name + ".position", position);
    shade.setVec3(name + ".ambient", ambient);
    shade.setVec3(name + ".diffuse", diffuse);
    shade.setVec3(name + ".specular", specular);
    shade.setFloat(name + ".constant", constant);
    shade.setFloat(name + ".linear", linear);
    shade.setFloat(name + ".quadratic", quadratic);
}

void SpotLight::setTo(GL::Shader& shade, std::string name) {
    shade.use();
    shade.setVec3(name + ".position", position);
    shade.setVec3(name + ".direction", position);
    shade.setVec3(name + ".ambient", ambient);
    shade.setVec3(name + ".diffuse", diffuse);
    shade.setVec3(name + ".specular", specular);
    shade.setFloat(name + ".constant", constant);
    shade.setFloat(name + ".linear", linear);
    shade.setFloat(name + ".quadratic", quadratic);
    shade.setFloat(name + ".cutOff", cutOff);
}
