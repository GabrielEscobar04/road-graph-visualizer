#include "Renderer.h"
#include <iostream>

/* CONSTRUCTORS */
Renderer::Renderer() {
    shader = std::make_unique<Shader>("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    setupOpenGL();
}

void Renderer::setupOpenGL() {
    glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer() {
    cleanupBuffers();
}

void Renderer::cleanupBuffers() {
    for (auto& buffer : buffers) {
        glDeleteVertexArrays(1, &buffer.vao);
        glDeleteBuffers(1, &buffer.vbo);
    }
}


/* SETTERS */
void Renderer::setClearColor(int r, int g, int b, int a) {
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void Renderer::setUniformColor(unsigned int bufferIndex, glm::vec3 color) {
    if (bufferIndex >= buffers.size()) {
        std::cerr << "Invalid buffer index" << std::endl;
        return;
    }

    buffers[bufferIndex].color = color;
}

void Renderer::setModelMatrix(glm::mat4 model) {
    this->model = model;
}

void Renderer::setViewMatrix(glm::mat4 view) {
    this->view = view;
}

void Renderer::setProjectionMatrix(glm::mat4 projection) {
    this->projection = projection;
}


/* METHODS */
unsigned int Renderer::createBuffer(const std::vector<float>& vertices, GLenum mode, float thickness, bool verticesHaveColor, glm::vec3 uniformColor) {
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    unsigned int vertexSize = verticesHaveColor ? 6 : 3;
    GLsizei stride = vertexSize * sizeof(float);
    
    GLuint positionLoc = shader->getAttributeLocation("vertexPosition");
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(positionLoc);

    if (verticesHaveColor) {
        GLuint colorLoc = shader->getAttributeLocation("vertexColor");
        glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(colorLoc);
    }

    glBindVertexArray(0);

    unsigned int attributeCount = vertices.size() / vertexSize;
    buffers.push_back({vao, vbo, mode, attributeCount, thickness, verticesHaveColor, uniformColor, vertexSize});
    
    return buffers.size() - 1;
}

void Renderer::updateBufferData(unsigned int bufferIndex, std::unordered_map<unsigned int, glm::vec3>& updates) {
    if (bufferIndex >= buffers.size()) {
        std::cerr << "Invalid buffer index" << std::endl;
        return;
    }

    Buffer& buffer = buffers[bufferIndex];
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    for (const auto& [vertexIndex, color] : updates) {
        unsigned int offset = vertexIndex * 6 + 3;  // +3 to skip position data
        glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(float), 3 * sizeof(float), &color[0]);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::render() {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& buffer : buffers) {
        shader->use();
        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setBool("useVertexColor", buffer.verticesHaveColor);
        shader->setVec3("uniformColor", buffer.color);

        glBindVertexArray(buffer.vao);
        if (buffer.mode == GL_POINTS) {
            glPointSize(buffer.thickness);
            glDrawArrays(buffer.mode, 0, buffer.attributeCount);
        } else if (buffer.mode == GL_LINES) {
            glLineWidth(buffer.thickness);
            glDrawArrays(buffer.mode, 0, buffer.attributeCount);
        }
    }

    glBindVertexArray(0);
}