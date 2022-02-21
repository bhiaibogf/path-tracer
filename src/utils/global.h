//
// Created by bhiaibogf on 2022/2/18.
//

#ifndef PATH_TRACER_GLOBAL_H
#define PATH_TRACER_GLOBAL_H

#include <eigen3/Eigen/Eigen>

#include <iostream>
#include <cmath>
#include <random>

namespace global {
    const float kPi = M_PI;
    const Eigen::Vector3f kBlack = Eigen::Vector3f(0.f, 0.f, 0.f);

    typedef Eigen::Vector3f Color;
    typedef Eigen::Vector3f Vector;

    float Radius(float alpha);

    void UpdateProgress(float progress);

    namespace {
        std::random_device rd_;
        std::default_random_engine random_engine_;
        std::uniform_real_distribution<float> uniform_;
    }

    float RandomFloat();

}


#endif //PATH_TRACER_GLOBAL_H
