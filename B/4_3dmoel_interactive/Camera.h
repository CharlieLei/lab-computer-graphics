#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD, BACKWARD, LEFT, RIGHT
};

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 1.0f;
const float FOV = 45.0f;

class Camera {
public:
    // camera Attributes
    float radius;
    float theta;
    float phi;
    glm::vec3 Position;
    glm::vec3 TargetPos;
    glm::vec3 WorldUp;
    glm::vec3 Up;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Fov;

    // constructor with vectors
    Camera(float radius = 2.0, float theta = 90.0, float phi = 90.0,
           glm::vec3 WorldUp = glm::vec3(0.0, 1.0, 0.0));

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    void ChangePosition(float xoffset, float yoffset);

    void ChangeDistanceFromTarget(float offset);

    void ChangeFov(float yoffset);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};


#endif //LEARNOPENGL_CAMERA_H
