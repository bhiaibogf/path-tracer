//
// Created by bhiaibogf on 2022/2/25.
//

#ifndef PATH_TRACER_NODE_H
#define PATH_TRACER_NODE_H


#include "bound.h"
#include "../geometry/primitive.h"

class Node {
public:
    explicit Node(std::vector<const Primitive *> *primitive_1);

    ~Node() = default;

    auto area_weighted() const { return area_weighted_; }

    bool Intersect(Ray *ray, Intersection *intersection) const;

    void SampleLight(Intersection *intersection, float *pdf, float area) const;

private:
    static const int kMaxPrimitives;

    Bound bound_;
    std::vector<const Primitive *> primitives_;
    float area_weighted_;

    Node *left_;
    Node *right_;

};


#endif //PATH_TRACER_NODE_H
