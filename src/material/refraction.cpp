//
// Created by bhiaibogf on 2022/2/23.
//

#include "refraction.h"

Refraction::Refraction(float n_i_) : ior_(n_i_) {}

global::Color
Refraction::Albedo(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    float schlick = global::Schlick(wo, normal, ior_);

    if (wi == global::Refract(wo, normal, ior_)) {
        return {1 - schlick, 1 - schlick, 1 - schlick};
    } else if (wi == global::Reflect(wo, normal)) {
        return {schlick, schlick, schlick};
    }

    return global::kBlack;
}

global::Color Refraction::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return global::kBlack;
}

global::Vector Refraction::Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const {
    *pdf = global::kInf;
    global::Vector refract = global::Refract(wo, normal, ior_);
    if (refract != global::kNone) {
        return refract;
    } else {
        return global::Reflect(wo, normal);
    }
}

float Refraction::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return 0.f;
}

std::ostream &operator<<(std::ostream &os, const Refraction &refraction) {
    os << "Refraction:\n\tni = " << refraction.ior_;
    return os;
}
