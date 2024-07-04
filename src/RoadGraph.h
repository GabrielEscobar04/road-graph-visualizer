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
};

struct Path {
    std::vector<int> nodeIds;
};

class RoadGraph {
public:
    RoadGraph(const std::string& nodesFile, const std::string& edgesFile);

    // Getters
    const std::unordered_map<int, glm::vec3>& getNodes() const;
    const std::vector<Road>& getRoads() const;
    const std::unordered_map<std::string, Path>& getPaths() const;
    glm::vec3 getNodePosition(int id) const;
    glm::vec3 getCenter() const;
    float getRadius() const;

    // Query methods
    bool roadExists(int from, int to) const;
    bool nodeExists(int id) const;
    bool pathExists(const std::string& pathName) const;

    // Modifiers
    void addNode(int id, const glm::vec3& position);
    void addRoad(int from, int to, float meters, float maxSpeed, int lanes);
    void addPath(const std::string& pathName, const std::vector<int>& nodeIds);
    void removePath(const std::string& pathName);

private:
    std::unordered_map<int, glm::vec3> nodes;
    std::vector<Road> roads;
    std::unordered_map<int, std::vector<int>> adjacentNodes;
    std::unordered_map<std::string, Path> paths;

    glm::vec3 maxCoords;
    glm::vec3 minCoords;
    glm::vec3 center;

    void updateBoundingBox(const glm::vec3& position);
};