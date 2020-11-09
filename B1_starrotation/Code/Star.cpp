//
// Created by 01 on 2020/10/14.
//

#include "Star.h"

Star::Star(float spin, glm::vec3 Color)
        : spin(spin), Color(Color), type(ARCHIMEDEAN), isFermatRadiusNeg(false) {

    float vs[] = {
            1.0, 1.0, 0.0, 1.0, 1.0,
            -1.0, 1.0, 0.0, 0.0, 1.0,
            -1.0, -1.0, 0.0, 0.0, 0.0,
            1.0, -1.0, 0.0, 1.0, 0.0,
    };
    unsigned int is[] = {
            0, 1, 2,
            0, 2, 3
    };
    for (auto f: vs) vertices.push_back(f);
    for (auto ui: is) indices.push_back(ui);

    setup();
}

glm::vec3 Star::GetPosition() {
    float radius = 0.0;
    if (type == ARCHIMEDEAN) {
        float a = 0.05, b = 0.05;
        radius = a + b * spin;
    } else if (type == LOGARITHMIC) {
        float a = 0.2, b = 0.1;
        radius = a * exp(b * spin);
    } else if (type == FERMAT) {
        radius = 0.2 * sqrt(spin);
        radius = isFermatRadiusNeg? -radius: radius;
    }
    float starX = radius * cos(spin);
    float starY = radius * sin(spin);
    return glm::vec3(starX, starY, 0.0);
}

void Star::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Star::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(vertices[0]), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(vertices[0]), (void *) (3 * sizeof(vertices[0])));
    glEnableVertexAttribArray(1);
}

void Star::SetSpiralType(Spiral_Type newType) {
    type = newType;
}
