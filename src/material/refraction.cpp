//
// Created by bhiaibogf on 2022/2/23.
//

#include "refraction.h"

const float Refraction::n_s = 1e5f;

Refraction::Refraction(float n_i_) : ior_(n_i_) {}

global::Color Refraction::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    if (normal.dot(wo) * normal.dot(wi) < 0) {
        auto refract = global::Refract(wo, normal, ior_);
        float schlick = global::Schlick(wo, normal, ior_);
        float alpha = wi.dot(refract);
        if (alpha > 0) {
            return global::Color(1 - schlick, 1 - schlick, 1 - schlick) * (n_s + 2) * global::kInvTwoPi *
                   std::pow(alpha, n_s) / std::abs(normal.dot(wi));
        }
    }
    return global::kBlack;
}

global::Vector Refraction::Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const {
    global::Vector refract = global::Refract(wo, normal, ior_);
    if (refract != global::kNone) {
        auto xi_1 = generator::Rand(), xi_2 = generator::Rand();
        float z = std::pow(xi_1, 1.f / (n_s + 1.f));
        float r = std::sqrt(1.f - z * z);
        float phi = global::kTwoPi * xi_2;
        float sin_phi = std::sin(phi), cos_phi = std::cos(phi);
        global::Vector local(r * cos_phi, r * sin_phi, z);
        *pdf = (n_s + 1) * global::kInvTwoPi * std::pow(z, n_s);
        return ToWorld(local, refract);
    }
    return global::kNone;
}

float Refraction::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    global::Vector refract = global::Refract(wo, normal, ior_);
    if (refract != global::kNone) {
        float alpha = wi.dot(refract);
        if (alpha > 0) {
            return (n_s + 1) * global::kInvTwoPi * std::pow(alpha, n_s);
        }
    }
    return 0;
}

std::ostream &operator<<(std::ostream &os, const Refraction &refraction) {
    os << "Refraction:\n\tni = " << refraction.ior_;
    return os;
}
