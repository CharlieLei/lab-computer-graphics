//
// Created by 01 on 2020/10/21.
//

#include <GLFW/glfw3.h>
#include "Timer.h"

float Timer::deltaTime = 0.0;
float Timer::lastFrame = 0.0;

void Timer::refresh() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

