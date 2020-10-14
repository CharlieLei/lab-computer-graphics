#include <iostream>
#include "Camera.h"

Camera::Camera(float radius, float theta, float phi, glm::vec3 WorldUp)
        : radius(radius), theta(theta), phi(phi), WorldUp(WorldUp),
          MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV) {
    TargetPos = glm::vec3(0.0, 0.0, 0.0);
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, TargetPos, Up);
}

void Camera::ChangePosition(float xoffset, float yoffset) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    theta -= xoffset;
    phi += yoffset;
    if (phi < 0.001) phi = 0.001;
    else if (phi > 90.0) phi = 90.0;  // 防止万向节死锁

    updateCameraVectors();
}

void Camera::ChangeDistanceFromTarget(float offset) {
    radius -= offset;
    if (radius < 1.0) radius = 1.0;
    updateCameraVectors();
}

void Camera::ChangeFov(float yoffset) {
    Fov -= yoffset;
    if (Fov < 1.0f) Fov = 1.0f;
    else if (Fov > 45.0f) Fov = 45.0f;
}

void Camera::updateCameraVectors() {
    float camX = radius * sin(glm::radians(phi)) * cos(glm::radians(theta));
    float camY = radius * cos(glm::radians(phi));
    float camZ = radius * sin(glm::radians(phi)) *  sin(glm::radians(theta));
    Position = glm::vec3(camX, camY, camZ);

    glm::vec3 Front = glm::normalize(TargetPos - Position);
    glm::vec3 Right = glm::normalize( glm::cross(Front, WorldUp) );
    Up    = glm::normalize( glm::cross(Right, Front));
}
