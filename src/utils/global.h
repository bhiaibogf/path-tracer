//
// Created by bhiaibogf on 2022/2/18.
//

#ifndef PATH_TRACER_GLOBAL_H
#define PATH_TRACER_GLOBAL_H

#include <eigen3/Eigen/Eigen>

#include <cmath>
#include <iostream>

namespace global {
    const float kPi = M_PI;
    const float kTwoPi = M_PI * 2.f;
    const float kInvPi = M_1_PI;
    const float kInvTwoPi = M_1_PI / 2.f;

    typedef Eigen::Vector3f Color;
    typedef Eigen::Vector3f Vector;
    typedef Eigen::Vector2f TexCoord;

    std::ostream &operator<<(std::ostream &os, const Eigen::Vector3f &vector);

    std::ostream &operator<<(std::ostream &os, const Eigen::Vector2f &vector);

    Vector Product(const Vector &a, const Vector &b);

    const Color kBlack = Eigen::Vector3f(0.f, 0.f, 0.f);
    const Color kWhite = Eigen::Vector3f(1.f, 1.f, 1.f);

    const Vector kNone = Eigen::Vector3f(0.f, 0.f, 0.f);
    const TexCoord kNoTex = Eigen::Vector2f(0.f, 0.f);

    void UpdateProgress(float progress);

    float Radius(float alpha);

    Vector Reflect(const Vector &wi, const Vector &normal);

    Vector Refract(const Vector &wi, const Vector &normal, float ior);

    float Schlick(const Vector &wi, const Vector &normal, float ior);

    float PowerHeuristic(float pdf_1, float pdf_2, float beta = 2.f);

}


#endif //PATH_TRACER_GLOBAL_H
