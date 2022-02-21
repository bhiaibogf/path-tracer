//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_SCENE_H
#define PATH_TRACER_SCENE_H


#include "intersection.h"
#include "object.h"
#include "../material/material.h"
#include "../ray/ray.h"
#include "../utils/global.h"

#include <iostream>
#include <random>

class Scene {
public:
    Scene();

    ~Scene() = default;

    global::Color Trace(Ray *ray) const;

    void AddObject(Object *object) {
        objects_.push_back(object);
    }

private:
    const Eigen::Vector3f kBackgroundColor = global::kBlack;
    const float kEpsilon = 1e-6;
    const float kRussianRoulette = 0.8f;

    std::vector<Object *> objects_;

    bool Intersect(Ray *ray, Intersection *intersection) const;

    global::Color Shade(const Intersection &intersection) const;

    void SampleLight(Intersection *intersection, float *pdf) const;

    bool RussianRoulette() const;

};


#endif //PATH_TRACER_SCENE_H
