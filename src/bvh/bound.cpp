//
// Created by bhiaibogf on 2022/2/25.
//

#include "bound.h"

Bound::Bound() {
    float min_float = std::numeric_limits<float>::lowest();
    float max_float = std::numeric_limits<float>::max();
    min_ = global::Vector(max_float, max_float, max_float);
    max_ = global::Vector(min_float, min_float, min_float);
}

bool Bound::Intersect(const Ray &ray) const {
    float t_enter = -std::numeric_limits<float>::infinity(), t_exit = std::numeric_limits<float>::infinity();

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
