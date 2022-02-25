//
// Created by bhiaibogf on 2022/2/25.
//

#ifndef PATH_TRACER_BOUND_H
#define PATH_TRACER_BOUND_H


#include "../utils/global.h"
#include "../ray/ray.h"

class Bound {
public:
    Bound();

    Bound(const global::Vector &min, const global::Vector &max);

    ~Bound() = default;

    bool Intersect(const Ray &ray) const;

    int MaxExtent() const;

    global::Vector Centroid();

    Bound operator|(const Bound &other) const;

    Bound operator|=(const Bound &other);

    Bound operator+(const global::Vector &other) const;

    Bound operator+=(const global::Vector &other);

private:
    const float kMinFloat = std::numeric_limits<float>::lowest();
    const float kMaxFloat = std::numeric_limits<float>::max();

    global::Vector min_, max_;

    global::Vector Diagonal() const;

    static global::Vector Min(const global::Vector &a, const global::Vector &b);

    static global::Vector Max(const global::Vector &a, const global::Vector &b);

};


#endif //PATH_TRACER_BOUND_H
