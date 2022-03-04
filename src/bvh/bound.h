//
// Created by bhiaibogf on 2022/2/25.
//

#ifndef PATH_TRACER_BOUND_H
#define PATH_TRACER_BOUND_H


#include "../ray/ray.h"

class Bound {
public:
    Bound();

    Bound(global::Vector min, global::Vector max);

    ~Bound() = default;

    bool Intersect(const Ray &ray) const;

    int MaxExtent() const;

    global::Vector Centroid() const;

    global::Vector Diagonal() const;

    Bound operator|(const Bound &other) const;

    Bound operator|=(const Bound &other);

    Bound operator+(const global::Vector &other) const;

    Bound operator+=(const global::Vector &other);

private:
    static const float kMinFloat;
    static const float kMaxFloat;

    global::Vector min_, max_;

    static global::Vector Min(const global::Vector &a, const global::Vector &b);

    static global::Vector Max(const global::Vector &a, const global::Vector &b);

};


#endif //PATH_TRACER_BOUND_H
