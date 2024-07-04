#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void setClearColor(int r, int g, int b, int a);
    void setUniformColor(unsigned int bufferIndex, glm::vec3 color);
    void setModelMatrix(glm::mat4 model);
    void setViewMatrix(glm::mat4 view);
    void setProjectionMatrix(glm::mat4 projection);

    unsigned int createBuffer(const std::vector<float>& vertices, GLenum mode, float thickness = 1.0f, bool verticesHaveColor = false, glm::vec3 uniformColor = glm::vec3(0.0f));
    void updateBufferData(unsigned int bufferIndex, std::unordered_map<unsigned int, glm::vec3>& updates);
    void render();

private:
    struct Buffer {
        GLuint vao;
        GLuint vbo;
        GLenum mode;
        unsigned int attributeCount;
        float thickness;
        bool verticesHaveColor;
        glm::vec3 color;
        unsigned int vertexSize;
    };

    std::unique_ptr<Shader> shader;
    std::vector<Buffer> buffers;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

private:
    void setupOpenGL();
    void cleanupBuffers();
};