//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_SCENE_H
#define PATH_TRACER_SCENE_H


#include "intersection.h"
#include "../material/material.h"
#include "../ray/ray.h"
#include "../utils/funcs.h"

#include <eigen3/Eigen/Eigen>

#include <iostream>
#include <random>

class Scene {
public:
    Scene();

    ~Scene() = default;

    Eigen::Vector3f Trace(const Ray &ray) const;

private:
    const Eigen::Vector3f kBackgroundColor = Eigen::Vector3f(0.f, 0.f, 0.f);
    const Eigen::Vector3f kBlack = Eigen::Vector3f(0.f, 0.f, 0.f);
    const float kEpsilon = 1e-6;
    const float kRussianRoulette = 0.8f;

    bool Intersect(const Ray &ray, Intersection *intersection) const;

    Eigen::Vector3f Shade(const Intersection &intersection) const;

    void SampleLight(Intersection *intersection, float *pdf) const;

    bool RussianRoulette() const;

};


#endif //PATH_TRACER_SCENE_H
