//
// Created by bhiaibogf on 2022/2/25.
//

#include "bound.h"

const float Bound::kMinFloat = std::numeric_limits<float>::lowest();
const float Bound::kMaxFloat = std::numeric_limits<float>::max();

Bound::Bound() {
    min_ = global::Vector(kMaxFloat, kMaxFloat, kMaxFloat);
    max_ = global::Vector(kMinFloat, kMinFloat, kMinFloat);
}

Bound::Bound(global::Vector min, global::Vector max) : min_(std::move(min)), max_(std::move(max)) {}

bool Bound::Intersect(const Ray &ray) const {
    float t_enter = kMinFloat, t_exit = kMaxFloat;

    for (int i = 0; i < 3; i++) {
        float t_min = (min_[i] - ray.origin()[i]) / ray.direction()[i],
                t_max = (max_[i] - ray.origin()[i]) / ray.direction()[i];
        if (ray.direction()[i] < 0) {
            std::swap(t_min, t_max);
        }
        t_enter = std::max(t_enter, t_min), t_exit = std::min(t_exit, t_max);
    }

    return t_enter <= t_exit && t_enter < ray.t_max() && t_exit >= ray.t_min();
}

int Bound::MaxExtent() const {
    global::Vector d = Diagonal();
    if (d.x() > d.y() && d.x() > d.z()) {
        return 0;
    } else if (d.y() > d.z()) {
        return 1;
    } else {
        return 2;
    }
}

global::Vector Bound::Centroid() {
    return 0.5 * min_ + 0.5 * max_;
}

Bound Bound::operator|(const Bound &other) const {
    return {Min(min_, other.min_), Max(max_, other.max_)};
}

Bound Bound::operator|=(const Bound &other) {
    min_ = Min(min_, other.min_);
    max_ = Max(max_, other.max_);
    return *this;
}

Bound Bound::operator+(const global::Vector &other) const {
    return {Min(min_, other), Max(max_, other)};
}

Bound Bound::operator+=(const global::Vector &other) {
    min_ = Min(min_, other);
    max_ = Max(max_, other);
    return *this;
}

global::Vector Bound::Diagonal() const {
    return max_ - min_;
}

global::Vector Bound::Min(const global::Vector &a, const global::Vector &b) {
    return {std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z())};
}

global::Vector Bound::Max(const global::Vector &a, const global::Vector &b) {
    return {std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z())};
}
