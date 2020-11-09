//
// Created by 01 on 2020/10/14.
//

#ifndef INC_1_STARROTATION_STAR_H
#define INC_1_STARROTATION_STAR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

enum Spiral_Type {
    ARCHIMEDEAN, LOGARITHMIC, FERMAT
};

class Star {
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float spin;
    glm::vec3 Color;

    Spiral_Type type;
    bool isFermatRadiusNeg;

    Star(float spin = 0.0, glm::vec3 Color = glm::vec3(0.0, 0.0, 0.0));

    glm::vec3 GetPosition();
    void SetSpiralType(Spiral_Type newType);
    void Draw();

private:
    unsigned int VAO, VBO, EBO;
    void setup();
};


#endif //INC_1_STARROTATION_STAR_H
