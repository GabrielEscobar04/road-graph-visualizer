#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

struct Road {
    int from, to;
    float meters;
    float maxSpeed;
    int lanes;

    Road() {}
    Road(int from, int to, float meters, float maxSpeed, int lanes): from(from), to(to), meters(meters), maxSpeed(maxSpeed), lanes(lanes) {}
};

struct Node {
    glm::vec3 position;

    Node() {}
    Node(const glm::vec3& position): position(position) {}
};

class RoadGraph {
public:
    RoadGraph(const std::string& nodesFile, const std::string& edgesFile);

    // Getters
    const std::unordered_map<int, Node>& getNodes() const;
    const std::unordered_map<int, Road>& getRoads() const;
    glm::vec3 getNodePosition(int id) const;
    glm::vec3 getCenter() const;
    float getRadius() const;

    // Query methods
    bool roadExists(int from, int to) const;
    bool roadExists(int id) const;
    bool nodeExists(int id) const;

    // Modifiers
    void addNode(int id, const glm::vec3& position);
    void addRoad(int id, int from, int to, float meters, float maxSpeed, int lanes);

private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<int, Road> roads;
    std::unordered_map<int, std::vector<int>> adjacentNodes;

    glm::vec3 maxCoords;
    glm::vec3 minCoords;
    glm::vec3 center;

    void updateBoundingBox(const glm::vec3& position);
};