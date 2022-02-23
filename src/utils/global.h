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
    const float kPi2 = M_PI_2;
    const float k1PI = M_1_PI;
    const Eigen::Vector3f kBlack = Eigen::Vector3f(0.f, 0.f, 0.f);

    typedef Eigen::Vector3f Color;
    typedef Eigen::Vector3f Vector;
    typedef Eigen::Vector2f TexCoord;

    std::ostream &operator<<(std::ostream &os, const Eigen::Vector3f &vector);

    std::ostream &operator<<(std::ostream &os, const Eigen::Vector2f &vector);

    float Radius(float alpha);

    void UpdateProgress(float progress);

    namespace {
        std::random_device rd_;
        std::default_random_engine random_engine_(rd_());
        std::uniform_real_distribution<float> uniform_distribution_(0.f, 1.f);
        std::normal_distribution<float> normal_distribution_(0.f, 0.5f);
    }

    float Rand();

    float RandN();

    Vector Reflect(const Vector &wi, const Vector &normal);

    Vector Refract(const Vector &wi, const Vector &normal, float ior);

    float Schlick(const Vector &wi, const Vector &normal, float ior);

    Vector Product(const Vector &a, const Vector &b);

}


#endif //PATH_TRACER_GLOBAL_H
