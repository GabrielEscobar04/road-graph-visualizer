#include "Application.h"

/* CONSTRUCTORS */
Application::Application()
{
    Configuration& config = Configuration::getInstance();
    config.loadFromFile("config.txt");

    loadConfig();

    setupWindow();

    renderer = std::make_unique<Renderer>();

    roadGraph = std::make_unique<RoadGraph>(
        config.getValue<std::string>("nodesFile", "data/nodes.txt"),
        config.getValue<std::string>("edgesFile", "data/edges.txt")
    );

    camera = std::make_unique<Camera>(roadGraph->getCenter(), roadGraph->getRadius(), aspectRatio, fov);
}

void Application::loadConfig() {
    Configuration& config = Configuration::getInstance();

    windowWidth = config.getValue<int>("windowWidth", 800);
    windowHeight = config.getValue<int>("windowHeight", 600);
    windowTitle = config.getValue<std::string>("windowTitle", "Graph Visualizer");

    aspectRatio = (float)windowWidth / windowHeight;
    fov = config.getValue<float>("cameraFov", 45.0f);

    nodeSize = config.getValue<float>("nodeSize", 0.1f);
    edgeSize = config.getValue<float>("edgeSize", 0.1f);
}

void Application::setupWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

Application::~Application() {
    if (window) {
        glfwDestroyWindow(window);
    }
    
    renderer.reset();
    camera.reset();
    roadGraph.reset();

    glfwTerminate();
}


/* METHODS */
void printInstructions() {
    std::cout << "Use WASD keys to move around" << std::endl;
    std::cout << "Use up/down arrow keys to zoom in/out" << std::endl;
    std::cout << "Use left/right key to roll the camera" << std::endl;
    std::cout << "Press R key to reset camera view" << std::endl;
}

void Application::run() 
{
    printInstructions();

    std::vector<float> nodesVertices = getNodesBuffer(roadGraph->getNodes());
    std::vector<float> edgesVertices = getEdgesBuffer(roadGraph->getRoads());

    unsigned int nodesBufferIndex = renderer->createBuffer(nodesVertices, GL_POINTS, nodeSize, false, glm::vec3(0.0f, 0.0f, 0.0f));
    unsigned int edgesBufferIndex = renderer->createBuffer(edgesVertices, GL_LINES, edgeSize, true);

    while (!glfwWindowShouldClose(window)) {
        handleInput();
        updateCamera();

        renderer->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


/* PRIVATE METHODS */
std::vector<float> Application::getNodesBuffer(const std::unordered_map<int, Node>& nodes) {
    std::vector<float> vertices;
    for (const auto& nodePair : nodes) {
        Node node = nodePair.second;

        vertices.push_back(node.position.x);
        vertices.push_back(node.position.y);
        vertices.push_back(node.position.z);
    }

    return vertices;
}

std::vector<float> Application::getEdgesBuffer(const std::unordered_map<int, Road>& roads) {
    std::vector<float> vertices;
    for (const auto& roadPair : roads) {
        Road road = roadPair.second;

        glm::vec3 positionFrom = roadGraph->getNodePosition(road.from);
        glm::vec3 positionTo = roadGraph->getNodePosition(road.to);

        vertices.push_back(positionFrom.x); 
        vertices.push_back(positionFrom.y); 
        vertices.push_back(positionFrom.z); 
        vertices.push_back(defaultRoadColor.r);
        vertices.push_back(defaultRoadColor.g);
        vertices.push_back(defaultRoadColor.b);

        vertices.push_back(positionTo.x); 
        vertices.push_back(positionTo.y); 
        vertices.push_back(positionTo.z);
        vertices.push_back(defaultRoadColor.r);
        vertices.push_back(defaultRoadColor.g);
        vertices.push_back(defaultRoadColor.b);
    }

    return vertices;
}

void Application::handleInput() {
    if(glfwGetKey(window, GLFW_KEY_R)) {
        camera->reset();
    }

    if (glfwGetKey(window, GLFW_KEY_W)) {
        camera->moveUp(0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        camera->moveDown(0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_A)) {
        camera->moveLeft(0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_D)) {
        camera->moveRight(0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_UP)) {
        camera->decreaseFov(0.3f);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN)) {
        camera->increaseFov(0.3f);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT)) {
        camera->rotateRoll(-0.5f);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
        camera->rotateRoll(0.5f);
    }

    int newWidth, newHeight;
    glfwGetWindowSize(window, &newWidth, &newHeight);
    if (newWidth != windowWidth || newHeight != windowHeight) {
        windowWidth = newWidth;
        windowHeight = newHeight;
        aspectRatio = (float)windowWidth / windowHeight;
        glViewport(0, 0, windowWidth, windowHeight);
    }
}

void Application::updateCamera() {
    renderer->setViewMatrix(camera->getViewMatrix());
    renderer->setProjectionMatrix(camera->getProjectionMatrix());
    camera->setAspectRatio(aspectRatio);
}