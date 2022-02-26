//
// Created by bhiaibogf on 2022/2/25.
//

#ifndef PATH_TRACER_NODE_H
#define PATH_TRACER_NODE_H


#include "bound.h"
#include "../geometry/primitive.h"

class Node {
public:
    explicit Node(std::vector<Primitive *> *primitives);

    ~Node() = default;

    auto area() const { return area_; }

    bool Intersect(Ray *ray, Intersection *intersection) const;

    bool SampleLight(Intersection *intersection, float *pdf, float area) const;

private:
    Bound bound_;
    Primitive *primitive_;
    float area_;

    Node *left_;
    Node *right_;

};


#endif //PATH_TRACER_NODE_H
