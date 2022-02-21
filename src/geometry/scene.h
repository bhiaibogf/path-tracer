//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_SCENE_H
#define PATH_TRACER_SCENE_H


#include "../ray/ray.h"

#include <eigen3/Eigen/Eigen>

#include <iostream>

class Scene {
public:
    Scene();

    ~Scene() = default;

    Eigen::Vector3f Trace(const Ray &ray, int depth) const;

private:

};


#endif //PATH_TRACER_SCENE_H
