//
// Created by bhiaibogf on 2022/1/23.
//

#include "ray.h"

const float Ray::kEpsilon = 0.f;

Ray::Ray(const global::Vector &origin, const global::Vector &direction) : origin_(origin), direction_(direction) {
    t_min_ = kEpsilon;
    t_max_ = global::kInf;
}

global::Vector Ray::operator()(float t) const {
    return origin_ + direction_ * t;
}

std::ostream &operator<<(std::ostream &os, const Ray &ray) {
    using namespace global;
    os << ray.origin_ << " -> " << ray(ray.t_max_) << " : " << ray.direction_ << " * " << ray.t_max_ << "\n";
    return os;
}

bool Ray::Update(float t) {
    if (t > t_min_ && t < t_max_) {
        t_max_ = t;
        return true;
    }
    return false;
}
