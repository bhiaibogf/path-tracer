//
// Created by bhiaibogf on 2022/2/18.
//

#ifndef PATH_TRACER_FUNCS_H
#define PATH_TRACER_FUNCS_H

#include <iostream>
#include <cmath>
#include <random>

namespace funcs {
    const float kPi = M_PI;

    float Radius(float alpha);

    void UpdateProgress(float progress);

    namespace {
        std::random_device rd_;
        std::default_random_engine random_engine_;
        std::uniform_real_distribution<float> uniform_;
    }

    float RandomFloat();

}


#endif //PATH_TRACER_FUNCS_H
