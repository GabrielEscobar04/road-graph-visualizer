#include "Camera.h"
#include <algorithm>

#include <iostream>

Camera::Camera(glm::vec3 target, float radius, float aspectRatio, float fov)
    : target(target), radius(radius), aspectRatio(aspectRatio), fov(fov) {
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    position = target + glm::vec3(0.0f, 0.0f, 2 * radius);
    yaw = -90.0f;
    pitch = 0.0f;
    roll = 0.0f;
    zNear = 1 * radius;
    zFar = 3 * radius;

    initialPosition = position;
    initialYaw = yaw;
    initialPitch = pitch;
    initialRoll = roll;
    initialFov = fov;

    updateCameraVectors();
    updateProjectionMatrix();
    updateViewMatrix();
}

void Camera::reset() {
    position = initialPosition;
    yaw = initialYaw;
    pitch = initialPitch;
    roll = initialRoll;
    fov = initialFov;

    updateCameraVectors();
    updateProjectionMatrix();
    updateViewMatrix();
}

glm::mat4 Camera::getViewMatrix() const {
    return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

void Camera::setAspectRatio(float newAspectRatio) {
    aspectRatio = newAspectRatio;
    updateProjectionMatrix();
}

void Camera::moveLeft(float speed) {
    float scale = calculateMovementScale();
    position -= right * scale * speed;
    updateViewMatrix();
}

void Camera::moveRight(float speed) {
    float scale = calculateMovementScale();
    position += right * scale * speed;
    updateViewMatrix();
}

void Camera::moveUp(float speed) {
    float scale = calculateMovementScale();
    speed = std::clamp(speed, 0.01f, 1.0f);
    position += up * scale * speed;
    updateViewMatrix();
}

void Camera::moveDown(float speed) {
    float scale = calculateMovementScale();
    position -= up * scale * speed;
    updateViewMatrix();
}

void Camera::rotateYaw(float angle) {
    yaw += angle;
    updateCameraVectors();
}

void Camera::rotatePitch(float angle) {
    pitch += angle;
    pitch = std::clamp(pitch, -89.0f, 89.0f);
    updateCameraVectors();
}

void Camera::rotateRoll(float angle) {
    roll -= angle;
    updateCameraVectors();
}

void Camera::decreaseFov(float angle) {
    fov = std::max(1.0f, fov - angle);
    updateProjectionMatrix();
}

void Camera::increaseFov(float angle) {
    fov = std::min(179.0f, fov + angle);
    updateProjectionMatrix();
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(roll), front);
    right = glm::vec3(rollMatrix * glm::vec4(right, 0.0f));
    up = glm::vec3(rollMatrix * glm::vec4(up, 0.0f));

    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    viewMatrix = glm::lookAt(position, position + front, up);
}

void Camera::updateProjectionMatrix() {
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
}

float Camera::calculateMovementScale() const {
    // Calculate the distance from the camera to the target
    float distanceToTarget = glm::length(position - target);
    
    // Calculate the visible width at the target distance
    float visibleWidth = 2.0f * distanceToTarget * tan(glm::radians(fov * 0.5f));
    
    // Base scale on the visible width
    // This value can be adjusted to fine-tune the movement scale
    float scale = visibleWidth / 10.0f;
    
    // Clamp the scale to a reasonable range
    return std::clamp(scale, 0.1f, 10000.0f);
}