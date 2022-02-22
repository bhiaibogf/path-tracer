//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_TRIANGLE_H
#define PATH_TRACER_TRIANGLE_H


#include <utility>

#include "primitive.h"

class Triangle : public Primitive {
public:
    Triangle(std::array<global::Vector, 3> vertices, std::array<global::Vector, 3> normals,
             std::array<global::TexCoord, 3> tex_coords) :
            vertices_(std::move(vertices)), normals_(std::move(normals)), tex_coords_(std::move(tex_coords)) {}

    ~Triangle() override = default;

    bool Intersect(Ray *ray, Intersection *intersection) const override;

    void Sample(Intersection *intersection, float *pdf) const override;

private:
    std::array<global::Vector, 3> vertices_, normals_;
    std::array<global::TexCoord, 3> tex_coords_;

};


#endif //PATH_TRACER_TRIANGLE_H
