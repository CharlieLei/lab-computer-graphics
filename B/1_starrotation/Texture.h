//
// Created by 01 on 2020/10/14.
//

#ifndef INC_1_STARROTATION_TEXTURE_H
#define INC_1_STARROTATION_TEXTURE_H

#include <glad/glad.h>

#include <string>
#include <iostream>

#include "stb_image.h"

class Texture {
public:
    unsigned int ID;
    Texture(std::string path);

    void use();

private:
    unsigned int loadTexture(std::string &path);
};


#endif //INC_1_STARROTATION_TEXTURE_H
