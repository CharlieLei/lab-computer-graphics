//
// Created by 01 on 2020/10/21.
//

#ifndef INC_6_TERRAINENGINE_TEXTURE_H
#define INC_6_TERRAINENGINE_TEXTURE_H


#include <glad/glad.h>

class Texture {
public:
    static unsigned int loadTexture(const char *path, GLenum param);
};


#endif //INC_6_TERRAINENGINE_TEXTURE_H
