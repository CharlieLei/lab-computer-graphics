#include <iostream>
#include <string>

using namespace std;

#include "raytracer.h"

int main() {
    Raytracer *raytracer = new Raytracer;

    string scenePath, outputPath;
    cout << "scene txt[final.txt]: ";
    getline(cin, scenePath);
    cout << "output path[picture.bmp]: ";
    getline(cin, outputPath);

    scenePath = scenePath.empty() ? "final.txt" : scenePath;
    outputPath = outputPath.empty() ? "picture.bmp" : outputPath;

    raytracer->SetInput(scenePath);
    raytracer->SetOutput(outputPath);
//	raytracer->Run();
    raytracer->MultiThreadRun();
//	raytracer->DebugRun(740,760,410,430);
    return 0;
}
