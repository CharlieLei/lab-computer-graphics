//
// Created by 01 on 2020/10/19.
//

#ifndef INC_6_TERRAINENGINE_SKYBOX_H
#define INC_6_TERRAINENGINE_SKYBOX_H

#include "Shader.h"
#include "Camera.h"

const int DIMENSION = 30;
const int FACES = 5;
const float LENGTH = 50.0;
const float WIDTH = 60.0;
const float HEIGHT = 30.0;

class Skybox {
public:
    unsigned int IDs[FACES]; //frontID, backID, leftID, rightID, UpID;
    unsigned int waveID;

    Skybox();

    void setTexture(Shader &shader);

    void DrawSky(Shader &shader, Camera &camera);

    void DrawWave(Shader &shader, Camera &camera);

    void DrawReflection(Shader &shader, Camera &camera);

private:
    unsigned int VAOs[FACES], VBOs[FACES];
    unsigned int waveVAO, waveVBO;

    std::vector<std::string> paths = {
            "../texture/Skybox/SkyBox0.bmp",
            "../texture/Skybox/SkyBox2.bmp",
            "../texture/Skybox/SkyBox3.bmp",
            "../texture/Skybox/SkyBox1.bmp",
            "../texture/Skybox/SkyBox4.bmp"
    };

    std::string wavePath = "../texture/Skybox/SkyBox5.bmp";

    float vertices[FACES][DIMENSION] = {
            // front
            {
                    LENGTH,  2 * HEIGHT, -WIDTH, 1.0, 1.0,
                    LENGTH,  0.0,        -WIDTH, 1.0, 0.0,
                    -LENGTH, 0.0,        -WIDTH, 0.0, 0.0,
                    -LENGTH, 0.0,        -WIDTH, 0.0, 0.0,
                    -LENGTH, 2 * HEIGHT, -WIDTH, 0.0, 1.0,
                    LENGTH,  2 * HEIGHT, -WIDTH, 1.0, 1.0
            },
            // back
            {
                    -LENGTH, 2 * HEIGHT, WIDTH,  1.0, 1.0,
                    -LENGTH, 0.0,        WIDTH,  1.0, 0.0,
                    LENGTH,  0.0,        WIDTH,  0.0, 0.0,
                    LENGTH,  0.0,        WIDTH,  0.0, 0.0,
                    LENGTH,  2 * HEIGHT, WIDTH,  0.0, 1.0,
                    -LENGTH, 2 * HEIGHT, WIDTH,  1.0, 1.0
            },
            // left
            {
                    -LENGTH, 2 * HEIGHT, -WIDTH, 1.0, 1.0,
                    -LENGTH, 0.0,        -WIDTH, 1.0, 0.0,
                    -LENGTH, 0.0,        WIDTH,  0.0, 0.0,
                    -LENGTH, 0.0,        WIDTH,  0.0, 0.0,
                    -LENGTH, 2 * HEIGHT, WIDTH,  0.0, 1.0,
                    -LENGTH, 2 * HEIGHT, -WIDTH, 1.0, 1.0
            },
            // right
            {
                    LENGTH,  2 * HEIGHT, WIDTH,  1.0, 1.0,
                    LENGTH,  0.0,        WIDTH,  1.0, 0.0,
                    LENGTH,  0.0,        -WIDTH, 0.0, 0.0,
                    LENGTH,  0.0,        -WIDTH, 0.0, 0.0,
                    LENGTH,  2 * HEIGHT, -WIDTH, 0.0, 1.0,
                    LENGTH,  2 * HEIGHT, WIDTH,  1.0, 1.0
            },
            // up
            {
                    LENGTH,  2 * HEIGHT, WIDTH,  1.0, 1.0,
                    LENGTH,  2 * HEIGHT, -WIDTH, 1.0, 0.0,
                    -LENGTH, 2 * HEIGHT, -WIDTH, 0.0, 0.0,
                    -LENGTH, 2 * HEIGHT, -WIDTH, 0.0, 0.0,
                    -LENGTH, 2 * HEIGHT, WIDTH,  0.0, 1.0,
                    LENGTH,  2 * HEIGHT, WIDTH,  1.0, 1.0
            },
    };

    float waveVertices[DIMENSION] = {
            LENGTH, 0.0, -WIDTH, 10.0, 10.0,
            LENGTH, 0.0, WIDTH, 10.0, 0.0,
            -LENGTH, 0.0, WIDTH, 0.0, 0.0,
            -LENGTH, 0.0, WIDTH, 0.0, 0.0,
            -LENGTH, 0.0, -WIDTH, 0.0, 10.0,
            LENGTH, 0.0, -WIDTH, 10.0, 10.0,
    };

    void setupVertices();

    void setupWaveVertices();
};


#endif //INC_6_TERRAINENGINE_SKYBOX_H
