#include <iostream>
#include <string>
#include <ctime>

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

    clock_t startTime,endTime;
    startTime = clock();
    std::cout << "Start Time: " << startTime << std::endl;

//	raytracer->Run();
    raytracer->MultiThreadRun();
//	raytracer->DebugRun(740,760,410,430);

    endTime = clock();
    std::cout << "End Time: " << endTime << std::endl;
    std::cout << "Total Time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;

    return 0;
}
