//
// Created by bhiaibogf on 2022/2/23.
//

#include "refraction.h"

Refraction::Refraction(float n_i_) : ior_(n_i_) {}

global::Color Refraction::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    if (wi == global::Refract(wo, normal, ior_)) {
        // float schlick = global::Schlick(wo, normal, ior_);
        // return {1 - schlick, 1 - schlick, 1 - schlick};
        return {1, 1, 1};
    } else {
        return global::kBlack;
    }
}

global::Vector Refraction::Sample(const global::Vector &wo, const global::Vector &normal) const {
    return global::Refract(wo, normal, ior_);
}

float Refraction::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return 1.f;
}

std::ostream &operator<<(std::ostream &os, const Refraction &refraction) {
    os << "Refraction:\n\tni = " << refraction.ior_;
    return os;
}
