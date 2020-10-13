//
// Created by 01 on 2020/10/13.
//

#include "ObjLoader.h"

bool ObjLoader::LoadFile(std::string path, Model &model) {
    // 根据后辍判断是否为obj文件
    if (path.substr(path.size() - 4, 4) != ".obj")
        return false;

    std::ifstream file(path);

    if (!file.is_open())
        return false;

    std::vector<Vertex> aVertices;
    std::vector<unsigned int> aIndices;

    std::string currLine, keyword;
    while(std::getline(file, currLine)) {
        std::istringstream iss(currLine);
        iss >> keyword;
        if (keyword == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            Vertex vertex{};
            vertex.Position = glm::vec3(x, y, z);
            aVertices.push_back(vertex);
        }
        if (keyword == "f") {
            int a, b, c; // obj文件中面的下标从1开始
            iss >> a >> b >> c;
            aIndices.push_back(a);
            aIndices.push_back(b);
            aIndices.push_back(c);
        }
    }

    model.vertices = aVertices;
    model.indices = aIndices;

    return true;
}

std::string ObjLoader::firstToken(const std::string &in) {
    if (in.empty()) return "";

    size_t tokenStart = in.find_first_not_of(" \t");
    size_t tokenEnd = in.find_first_of(" \t", tokenStart);
    if (tokenStart != std::string::npos && tokenEnd != std::string::npos) {
        return in.substr(tokenStart, tokenEnd - tokenStart);
    } else if (tokenStart != std::string::npos) {
        return in.substr(tokenStart);
    }
}
