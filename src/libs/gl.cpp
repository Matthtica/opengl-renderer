#include "sdl.hpp"
#include <gl.hpp>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image.h>

namespace GL {
    using namespace glm;

    void Shader::logShader(Uint32 shader, const char* type) {
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            printf("ERROR::%s::SHADER::COMPILATION_FAILED\n", type);
            printf("%s", infoLog);
        }
    }

    void Shader::logProgram() {
        int success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            printf("ERROR::SHADER::PROGRAM:::LINKING_FAILED\n");
            printf("%s", infoLog);
        }
    }

    Shader::Shader(Shader&& other) noexcept {
        program = std::exchange(other.program, 0);
    }

    Shader& Shader::operator=(Shader &&other) noexcept {
        program = std::exchange(other.program, 0);
        return *this;
    }

    Shader::Shader(std::string vertexFile, std::string fragmentFile, std::string geometryFile) {
        using namespace std;

        ifstream vertex_stream(vertexFile);
        ifstream fragment_stream(fragmentFile);
        stringstream ss[2];
        ss[0] << vertex_stream.rdbuf();
        ss[1] << fragment_stream.rdbuf();
        string vertex_str = ss[0].str();
        string fragment_str = ss[1].str();
        const char* vertex_shader_source = vertex_str.c_str();
        const char* fragment_shader_source = fragment_str.c_str();

        Uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
        Uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(vertexShader, 1, &vertex_shader_source, NULL);
        glShaderSource(fragmentShader, 1, &fragment_shader_source, NULL);
        glCompileShader(vertexShader);
        logShader(vertexShader, "VERTEX");
        glCompileShader(fragmentShader);
        logShader(fragmentShader, "FRAGMENT");

        program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        uint32_t geometryShader = geometryFile.empty() ? 0 : glCreateShader(GL_GEOMETRY_SHADER);
        if (geometryFile != "") {
            ifstream geometry_stream(geometryFile);
            stringstream sg;
            sg << geometry_stream.rdbuf();
            string geometry_str = sg.str();
            const char* geometry_shader_source = geometry_str.c_str();

            glShaderSource(geometryShader, 1, &geometry_shader_source, NULL);
            glCompileShader(geometryShader);
            logShader(geometryShader, "GEOMETRY");
            glAttachShader(program, geometryShader);
        }

        glLinkProgram(program);
        logProgram();
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if (!geometryFile.empty()) glDeleteShader(geometryShader);
    }

    void Shader::use() const {
        glUseProgram(program);
    }

    int Shader::getUniformLocation(const char* name) {
        return glGetUniformLocation(program, name);
    }

    void Shader::setBool(const std::string&& name, bool value) const {
        glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string&& name, int value) const {
        glUniform1i(glGetUniformLocation(program, name.c_str()), value);
    }

    void Shader::setFloat(const std::string&& name, float value) const {
        glUniform1f(glGetUniformLocation(program, name.c_str()), value);
    }

    void Shader::setVec2(const std::string&& name, const vec2& value) const {
        glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec3(const std::string&& name, const vec3& value) const {
        glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec4(const std::string&& name, const vec4& value) const {
        glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
    }

    void Shader::setMat2(const std::string&& name, const mat2& value) const {
        glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }
    
    void Shader::setMat3(const std::string&& name, const mat3& value) const {
        glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }
    
    void Shader::setMat4(const std::string&& name, const mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    void Shader::setMVP(const mat4& model, const mat4& view, const mat4& projection) const {
        setMat4("model", model);
        setMat4("view", view);
        setMat4("projection", projection);
    }

    void Shader::setMV(const mat4& model, const mat4& view) const {
        setMat4("model", model);
        setMat4("view", view);
    }

    void Shader::setVP(const mat4& view, const mat4& projection) const {
        setMat4("view", view);
        setMat4("projection", projection);
    }

    void swap(Shader& a, Shader& b) noexcept {
        std::swap(a.program, b.program);
    }

    Shader::~Shader() {
        glDeleteProgram(program);
    }

    Camera::Camera(vec3 pos, vec3 u, float y, float p):
        front({0.0f, 0.0f, -1.0f}), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), position(pos), worldup(u), yaw(y), pitch(p)
    {
        Camera::updateCameraVectors();
    }

    mat4 Camera::GetViewMatrix() {
        mat4 transform = translate(mat4(1.0f), position);
        return inverse(transform);
        //return lookAt(position, position + front, up);
    }

    void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
        float velocity = movement_speed * deltaTime;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
    }

    void Camera::ProcessMouseMovement(vec2 offset) {
        offset *= mouse_sensitivity;
        yaw -= offset.x;
        pitch += offset.y;

        Camera::updateCameraVectors();
    }

    void Camera::updateCameraVectors() {
        front.x = cos(radians(yaw)) * cos(radians(pitch));
        front.y = sin(radians(pitch));
        front.z = sin(radians(yaw)) * cos(radians(pitch));
        front = glm::normalize(front);

        right = normalize(cross(front, worldup));
        up = normalize(cross(right, front));
    }

    uint32_t TextureFromFile(const char* path, const std::string& directory) {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;
        return SDL::loadTexture(filename.c_str());
    }

    Texture LoadTexture(const char* path, std::string type, bool flip) {
        Texture texture;
        texture.id = SDL::loadTexture(path, flip);
        texture.path = path;
        texture.type = type;
        return texture;
    }

    uint32_t create_ebo(std::vector<uint32_t>& indices) {
        uint32_t ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
        return ebo;
    }

    uint32_t create_vao() {
        uint32_t vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        return vao;
    }

    // TODO: Create a class for Uniform buffer object to manage everything about it.
    // TODO: non templated, member templated (Not needed may be);
    // TODO: didn't work tested
    uint32_t create_ubo(uint32_t byte_size) {
        uint32_t ubo;
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, byte_size, NULL, GL_STATIC_DRAW);
        return ubo;
    }

    void set_uniform_buffer(uint32_t ubo, uint32_t offset_byte, uint32_t byte_size, float* data) {
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, offset_byte, byte_size, data);
    }

    void bind_vbo(const uint32_t& vbo) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    }

    void bind_ebo(const uint32_t& ebo) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    }

    void bind_vao(const uint32_t& vao) {
        glBindVertexArray(vao);
    }
}
