//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_INTERSECTION_H
#define PATH_TRACER_INTERSECTION_H


#include "../material/material.h"

#include <eigen3/Eigen/Eigen>

struct Intersection {
    Material *material;
    Eigen::Vector3f position, normal, direction;
};


#endif //PATH_TRACER_INTERSECTION_H
