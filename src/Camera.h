#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 target, float radius, float aspectRatio, float fov);
    ~Camera() = default;

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }
    glm::vec3 getUp() const { return up; }
    float getFov() const { return fov; }
    float getZNear() const { return zNear; }
    float getZFar() const { return zFar; }

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void setAspectRatio(float aspectRatio);

    void moveLeft(float speed);
    void moveRight(float speed);
    void moveUp(float speed);
    void moveDown(float speed);

    void rotateYaw(float angle);
    void rotatePitch(float angle);
    void rotateRoll(float angle);

    void decreaseFov(float angle);
    void increaseFov(float angle);

    void reset();

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float roll;
    float zNear;
    float zFar;
    float fov;
    float aspectRatio;
    float radius;

    glm::vec3 initialPosition;
    float initialYaw;
    float initialPitch;
    float initialRoll;
    float initialFov;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void updateCameraVectors();
    void updateViewMatrix();
    void updateProjectionMatrix();
    float calculateMovementScale() const;
};