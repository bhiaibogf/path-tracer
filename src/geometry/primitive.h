//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_PRIMITIVE_H
#define PATH_TRACER_PRIMITIVE_H


#include "../utils/global.h"
#include "../ray/ray.h"
#include "../ray/intersection.h"

class Primitive {
public:
    Primitive() = default;

    virtual ~Primitive() = default;

    auto area() const { return area_; }

    virtual bool Intersect(Ray *ray, Intersection *intersection) const = 0;

    virtual void Sample(Intersection *intersection, float *pdf) const = 0;

protected:
    float area_;

};


#endif //PATH_TRACER_PRIMITIVE_H
