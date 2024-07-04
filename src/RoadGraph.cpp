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
    while (std::getline(edgesStream, line)) {
        std::istringstream iss(line);
        int idNode1, idNode2;
        float meters, maxSpeed;
        int lanes, oneWay;

        if (!(iss >> idNode1 >> idNode2 >> meters >> maxSpeed >> lanes >> oneWay)) {
            std::cerr << "Failed to parse edge line: " << line << std::endl;
            continue;
        }
        
        addRoad(idNode1, idNode2, meters, maxSpeed, lanes);

        if (!oneWay) {
            addRoad(idNode2, idNode1, meters, maxSpeed, lanes);
        }
    }

    center = (minCoords + maxCoords) / 2.0f;
}

const std::unordered_map<int, glm::vec3>& RoadGraph::getNodes() const {
    return nodes;
}

const std::vector<Road>& RoadGraph::getRoads() const {
    return roads;
}

const std::unordered_map<std::string, Path>& RoadGraph::getPaths() const {
    return paths;
}

glm::vec3 RoadGraph::getNodePosition(int id) const {
    auto it = nodes.find(id);
    if (it != nodes.end()) {
        return it->second;
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

bool RoadGraph::nodeExists(int id) const {
    return nodes.find(id) != nodes.end();
}

bool RoadGraph::pathExists(const std::string& pathName) const {
    return paths.find(pathName) != paths.end();
}

void RoadGraph::addNode(int id, const glm::vec3& position) {
    nodes[id] = position;
    updateBoundingBox(position);
}

void RoadGraph::addRoad(int from, int to, float meters, float maxSpeed, int lanes) {
    adjacentNodes[from].push_back(to);
    roads.push_back({from, to, meters, maxSpeed, lanes});
}

void RoadGraph::addPath(const std::string& pathName, const std::vector<int>& nodeIds) {
    if (paths.find(pathName) != paths.end()) {
        throw std::runtime_error("Path with this name already exists");
    }
    paths[pathName] = Path{nodeIds};
}

void RoadGraph::removePath(const std::string& pathName) {
    auto it = paths.find(pathName);
    if (it != paths.end()) {
        paths.erase(it);
    } else {
        throw std::runtime_error("Path not found");
    }
}

void RoadGraph::updateBoundingBox(const glm::vec3& position) {
    maxCoords = glm::max(maxCoords, position);
    minCoords = glm::min(minCoords, position);
}