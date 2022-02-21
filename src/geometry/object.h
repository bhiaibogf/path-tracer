//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_OBJECT_H
#define PATH_TRACER_OBJECT_H


#include "../utils/global.h"
#include "../material/material.h"
#include "intersection.h"

class Object {
public:
    Object() = default;

    virtual ~Object() = default;

    auto material() const { return material_; }

    auto area() const { return area_; }

    void Sample(Intersection *intersection, float *pdf);

private:
    Material *material_;
    float area_;

};


#endif //PATH_TRACER_OBJECT_H
