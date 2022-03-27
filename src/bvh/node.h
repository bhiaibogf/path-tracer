//
// Created by bhiaibogf on 2022/2/25.
//

#ifndef PATH_TRACER_NODE_H
#define PATH_TRACER_NODE_H


#include "bound.h"
#include "../geometry/primitive.h"

enum SplitMethod {
    kMiddle,
    kEqualCount,
    kSah,
    kMix
};

class Node {
public:
    explicit Node(std::vector<const Primitive *> *primitives, SplitMethod split_method, int depth = 0);

    ~Node() = default;

    auto area_weighted() const { return area_weighted_; }

    auto bound() const { return bound_; }

    bool Intersect(Ray *ray, Intersection *intersection) const;

    void SampleLight(Intersection *intersection, float *pdf, float area) const;

private:
    static const std::size_t kMaxPrimitives;
    static const std::size_t kUseSah;
    static const int kDepth;

    Bound bound_;
    std::vector<const Primitive *> primitives_;
    float area_weighted_;

    Node *left_;
    Node *right_;

};


#endif //PATH_TRACER_NODE_H
