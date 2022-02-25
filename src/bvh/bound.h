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

    Bound(global::Vector min, global::Vector max) : min_(min), max_(max) {}

    ~Bound() = default;

    bool Intersect(const Ray &ray) const;

    int MaxExtent() const {
        global::Vector d = Diagonal();
        if (d.x() > d.y() && d.x() > d.z())
            return 0;
        else if (d.y() > d.z())
            return 1;
        else
            return 2;
    }

    global::Vector Centroid() { return 0.5 * min_ + 0.5 * max_; }

    Bound operator|(const Bound &other) const {
        return {Min(min_, other.min_), Max(max_, other.max_)};
    }

    Bound operator+(const global::Vector &other) const {
        return {Min(min_, other), Max(max_, other)};
    }

    Bound operator|=(const Bound &other) {
        min_ = Min(min_, other.min_);
        max_ = Max(max_, other.max_);
        return *this;
    }

    Bound operator+=(const global::Vector &other) {
        min_ = Min(min_, other);
        max_ = Max(max_, other);
        return *this;
    }

private:
    global::Vector min_, max_;

    global::Vector Diagonal() const { return max_ - min_; }

    global::Vector Min(const global::Vector &a, const global::Vector &b) const {
        return global::Vector(std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z()));
    }

    global::Vector Max(const global::Vector &a, const global::Vector &b) const {
        return global::Vector(std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z()));
    }


};


#endif //PATH_TRACER_BOUND_H
