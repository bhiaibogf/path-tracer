//
// Created by bhiaibogf on 2022/1/23.
//

#include "ray.h"

const float Ray::kEpsilon = 1e-4f;

Ray::Ray(const Eigen::Vector3f &origin, const Eigen::Vector3f &direction) : origin_(origin), direction_(direction) {
    t_min_ = kEpsilon;
    t_max_ = std::numeric_limits<float>::max();
}

Eigen::Vector3f Ray::operator()(float t) const {
    return origin_ + direction_ * t;
}

std::ostream &operator<<(std::ostream &os, const Ray &ray) {
    os << "(" << ray.origin_ << ")->(" << ray.direction_ << "): " << ray.t_min_ << "\n";
    return os;
}

bool Ray::Update(float t) {
    if (t > t_min_ && t < t_max_) {
        t_max_ = t;
        return true;
    }
    return false;
}
