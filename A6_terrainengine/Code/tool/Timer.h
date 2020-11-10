//
// Created by 01 on 2020/10/21.
//

#ifndef INC_6_TERRAINENGINE_TIMER_H
#define INC_6_TERRAINENGINE_TIMER_H


class Timer {
public:
    static float deltaTime;
    static float lastFrame;

    static void refresh();
};


#endif //INC_6_TERRAINENGINE_TIMER_H
