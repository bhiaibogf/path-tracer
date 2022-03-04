//
// Created by bhiaibogf on 2022/3/4.
//

#ifndef PATH_TRACER_SPHERE_H
#define PATH_TRACER_SPHERE_H


#include "primitive.h"

class Sphere : public Primitive {
public:
    Sphere(Object *object, global::Vector center, float radius);

    bool Intersect(Ray *ray, Intersection *intersection) const override;

    void Sample(Intersection *intersection, float *pdf) const override;

    void InsertTo(std::vector<const Primitive *> *primitives) const override;

private:
    global::Vector center_;
    float radius_;

};


#endif //PATH_TRACER_SPHERE_H
