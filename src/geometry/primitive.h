//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_PRIMITIVE_H
#define PATH_TRACER_PRIMITIVE_H


#include "../utils/global.h"
#include "../utils/generator.h"
#include "../ray/ray.h"
#include "../ray/intersection.h"
#include "../bvh/bound.h"

class Object;

class Primitive {
public:
    explicit Primitive(Object *object);

    virtual ~Primitive() = default;

    auto area() const { return area_; }

    float Weight() const;

    float AreaWeighted() const;

    auto bound() const { return bound_; }

    Material *material() const;

    virtual bool Intersect(Ray *ray, Intersection *intersection) const = 0;

    virtual void Sample(Intersection *intersection, float *pdf) const = 0;

    virtual void InsertTo(std::vector<const Primitive *> *primitives) const = 0;

protected:
    float area_;
    Bound bound_;
    Object *object_;

};


#endif //PATH_TRACER_PRIMITIVE_H
