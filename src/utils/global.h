//
// Created by bhiaibogf on 2022/2/18.
//

#ifndef PATH_TRACER_GLOBAL_H
#define PATH_TRACER_GLOBAL_H


#include <Eigen/Eigen>

#include <array>
#include <cmath>
#include <iostream>
#include <utility>

namespace global {
    const float kPi = M_PI;
    const float kTwoPi = M_PI * 2.f;
    const float kInvPi = M_1_PI;
    const float kInvTwoPi = M_1_PI / 2.f;
    const float kInf = std::numeric_limits<float>::infinity();

    typedef Eigen::Vector3f Vector;
    typedef Vector Color;
    typedef Eigen::Vector2f Vector2;
    typedef Vector2 TexCoord;

    typedef std::array<global::Vector, 3> VectorArray;
    typedef std::array<global::TexCoord, 3> TexCoordArray;

    std::ostream &operator<<(std::ostream &os, const Eigen::Vector3f &vector);

    std::ostream &operator<<(std::ostream &os, const Eigen::Vector2f &vector);

    Vector Product(const Vector &a, const Vector &b);

    template<typename T>
    T Mix(const T &a, const T &b, float ratio) {
        return a * (1 - ratio) + b * ratio;
    }

    const Vector kZero = Eigen::Vector3f::Zero();
    const Vector kOnes = Eigen::Vector3f::Ones();

    const Color kBlack = kZero;
    const Color kWhite = kOnes;

    const Vector kNone = kZero;
    const TexCoord kNoTex = Eigen::Vector2f::Zero();

    void UpdateProgress(float progress);

    float Radius(float alpha);

    float Degree(float alpha);

    Vector Reflect(const Vector &wi, const Vector &normal);

    Vector Refract(const Vector &wi, const Vector &normal, float ior);

    Vector ToWorld(const Vector &local, const Vector &normal);

    float Luminance(const Color &color);

    float Schlick(const Vector &wi, const Vector &normal, float ior);

    float PowerHeuristic(float pdf_sample, float pdf_other, float beta = 2.f);

}


#endif //PATH_TRACER_GLOBAL_H
