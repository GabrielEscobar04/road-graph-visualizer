#include "RoadGraph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <algorithm>

RoadGraph::RoadGraph(const std::string& nodesFile, const std::string& edgesFile) {
    std::ifstream nodesStream(nodesFile);
    std::ifstream edgesStream(edgesFile);

    if (!nodesStream.is_open()) {
        std::cerr << "Failed to open nodes file: " << nodesFile << std::endl;
        return;
    }
    if (!edgesStream.is_open()) {
        std::cerr << "Failed to open edges file: " << edgesFile << std::endl;
        return;
    }

    std::string line;

    // Initialize bounding box
    maxCoords = glm::vec3(-std::numeric_limits<float>::max());
    minCoords = glm::vec3(std::numeric_limits<float>::max());

    // Add nodes
    while (std::getline(nodesStream, line)) {
        std::istringstream iss(line);
        int id;
        float x, y, z;

        if (!(iss >> id >> x >> y >> z)) {
            std::cerr << "Failed to parse node line: " << line << std::endl;
            continue;
        }

        addNode(id, glm::vec3(x, y, z));
    }

    // Add edges
    int id = 0;
    while (std::getline(edgesStream, line)) {
        std::istringstream iss(line);
        int idNode1, idNode2;
        float meters, maxSpeed;
        int lanes, oneWay;

        if (!(iss >> idNode1 >> idNode2 >> meters >> maxSpeed >> lanes >> oneWay)) {
            std::cerr << "Failed to parse edge line: " << line << std::endl;
            continue;
        }
        
        addRoad(id, idNode1, idNode2, meters, maxSpeed, lanes);
        ++id;

        if (!oneWay) {
            addRoad(id, idNode2, idNode1, meters, maxSpeed, lanes);
            ++id;
        }
    }

    center = (minCoords + maxCoords) / 2.0f;
}

const std::unordered_map<int, Node>& RoadGraph::getNodes() const {
    return nodes;
}

const std::unordered_map<int, Road>& RoadGraph::getRoads() const {
    return roads;
}

glm::vec3 RoadGraph::getNodePosition(int id) const {
    auto it = nodes.find(id);
    if (it != nodes.end()) {
        Node node = it->second;
        return node.position;
    }
    throw std::runtime_error("Node not found");
}

glm::vec3 RoadGraph::getCenter() const {
    return center;
}

float RoadGraph::getRadius() const {
    return glm::distance(center, maxCoords);
}

bool RoadGraph::roadExists(int from, int to) const {
    auto it = adjacentNodes.find(from);
    if (it != adjacentNodes.end()) {
        const auto& nodes = it->second;
        return std::find(nodes.begin(), nodes.end(), to) != nodes.end();
    }
    return false;
}

bool RoadGraph::roadExists(int id) const {
    return roads.find(id) != roads.end();
}

bool RoadGraph::nodeExists(int id) const {
    return nodes.find(id) != nodes.end();
}

void RoadGraph::addNode(int id, const glm::vec3& position) {
    nodes[id] = position;
    updateBoundingBox(position);
}

void RoadGraph::addRoad(int id, int from, int to, float meters, float maxSpeed, int lanes) {
    adjacentNodes[from].push_back(to);

    Road road = Road(from, to, meters, maxSpeed, lanes);
    roads[id] = road;
}

void RoadGraph::updateBoundingBox(const glm::vec3& position) {
    maxCoords = glm::max(maxCoords, position);
    minCoords = glm::min(minCoords, position);
}