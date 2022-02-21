//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_TRIANGLE_H
#define PATH_TRACER_TRIANGLE_H


#include <utility>

#include "primitive.h"

class Triangle : public Primitive {
public:
    Triangle(std::array<global::Vector, 3> vertices, std::array<global::Vector, 3> normals) :
            vertices_(std::move(vertices)), normals_(std::move(normals)) {}

    ~Triangle() override = default;

    bool Intersect(Ray *ray, Intersection *intersection) const override;

    void Sample(Intersection *intersection, float *pdf) const override;

private:
    std::array<global::Vector, 3> vertices_, normals_;

};


#endif //PATH_TRACER_TRIANGLE_H
