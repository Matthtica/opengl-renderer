#pragma once
#include <glm/vec3.hpp>
#include <string>
#include "gl.hpp"

using namespace glm;
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    DirectionalLight(vec3 dir = {-0.5f, -0.5f, -0.5}, vec3 am = {0.1f, 0.1f, 0.1f}, vec3 diff = {0.5f, 0.5f, 0.5f}, vec3 spec = {1.0f, 1.0f, 1.0f}):
        direction(dir), ambient(am), diffuse(diff), specular(spec) {}

    void Imgui(std::string&& title);

    void setTo(GL::Shader& shade, std::string name);
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    PointLight(
            vec3 position = {0.0f, 0.0f, 0.0f}, vec3 ambient = {0.1f, 0.1f, 0.1f},
            vec3 diffuse = {0.5f, 0.5f, 0.5f}, vec3 specular = {1.0f, 1.0f, 1.0f},
            float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f):
        position(position), ambient(ambient), diffuse(diffuse), specular(specular),
        constant(constant), linear(linear), quadratic(quadratic) { }

    void Imgui(std::string&& title);

    void setTo(GL::Shader& shade, std::string name) const;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    float cutOff;

    void setTo(GL::Shader& shade, std::string name);
};
