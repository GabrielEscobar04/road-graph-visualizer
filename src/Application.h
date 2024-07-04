#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include "glfw/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RoadGraph.h"
#include "Camera.h"
#include "Renderer.h"
#include "Configuration.h"

struct Color {
    float r, g, b;
    Color(float r, float g, float b): r(r), g(g), b(b) {}
};

class Application {
public:
    Application();
    ~Application();

    void run();

private:
    GLFWwindow* window;
    std::unique_ptr<RoadGraph> roadGraph;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Renderer> renderer;

    int windowWidth, windowHeight;
    std::string windowTitle;
    
    float aspectRatio;
    float fov;

    float nodeSize;
    float edgeSize;

    Color defaultRoadColor = Color(0.0f, 0.0f, 0.0f);

private:
    void loadConfig();
    void setupWindow();

    void handleInput();
    void updateCamera();
    std::vector<float> getNodesBuffer(const std::unordered_map<int, glm::vec3>& nodes);
    std::vector<float> getEdgesBuffer(const std::vector<Road>& roads);
};