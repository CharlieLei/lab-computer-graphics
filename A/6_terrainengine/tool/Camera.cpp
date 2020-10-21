#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const unsigned int scrWidth, const unsigned int scrHeight,
               glm::vec3 position, glm::vec3 up, float yaw, float pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    ScrWidth = scrWidth;
    ScrHeight = scrHeight;

    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    Aspect = (float) ScrWidth / (float) ScrHeight;

    LastX = (float) ScrWidth / 2.0;
    LastY = (float) ScrHeight / 2.0;

    FirstMouse = true;

    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() {
    return glm::perspective(glm::radians(Zoom), Aspect, 0.1f, 200.0f);
}

void Camera::ProcessKeyboard(Camera_Movement direction) {
    float velocity = MovementSpeed * Timer::deltaTime;
    if (direction == FORWARD) Position += velocity * Front;
    else if (direction == BACKWARD) Position -= velocity * Front;
    else if (direction == LEFT) Position -= velocity * Right;
    else if (direction == RIGHT) Position += velocity * Right;
}

void Camera::ProcessMouseMovement(double xpos, double ypos, bool constrainPitch) {
    if (FirstMouse) {
        LastX = xpos;
        LastY = ypos;
        FirstMouse = false;
    }

    float xoffset = xpos - LastX;
    float yoffset = LastY - ypos; // reversed since y-coordinates go from bottom to top

    LastX = xpos;
    LastY = ypos;

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f) Pitch = 89.0f;
        else if (Pitch < -89.0f) Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(double yoffset) {
    Zoom -= yoffset;
    if (Zoom < 1.0f) Zoom = 1.0f;
    else if (Zoom > 45.0f) Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
    newFront.y = sin(glm::radians(Pitch));
    newFront.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
    Front = newFront;
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
