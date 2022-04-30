//
// Created by bhiaibogf on 2022/2/24.
//

#include "generator.h"

float generator::Rand() {
    return uniform_distribution_(random_engine_);
}

float generator::RandN() {
    return normal_distribution_(random_engine_);
}

global::Vector2 generator::SampleDisk() {
    float xi_1 = Rand(), xi_2 = Rand();
    float r = std::sqrt(xi_1), theta = global::kTwoPi * xi_2;
    return {r * std::cos(theta), r * std::sin(theta)};
}
