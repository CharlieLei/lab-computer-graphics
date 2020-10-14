//
// Created by 01 on 2020/10/13.
//

#ifndef INC_3_3DMODEL_OBJLOADER_H
#define INC_3_3DMODEL_OBJLOADER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>

#include "Model.h"

class ObjLoader {
public:

    bool LoadFile(std::string path, Model &model);

private:
    std::string firstToken(const std::string &in);
};


#endif //INC_3_3DMODEL_OBJLOADER_H
