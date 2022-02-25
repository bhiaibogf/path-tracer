//
// Created by bhiaibogf on 2022/2/22.
//

#include "lambert.h"

Lambert::Lambert(const global::Color &k_d) : albedo_(k_d) {}

global::Color Lambert::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    if (normal.dot(wo) > 0 && normal.dot(wi) > 0) {
        return albedo_ * global::kInvPi;
    }
    return global::kBlack;
}

global::Vector Lambert::Sample(const global::Vector &wo, const global::Vector &normal) const {
    if (normal.dot(wo) > 0) {
        auto xi_1 = generator::Rand(), xi_2 = generator::Rand();
        float z = std::sqrt(1.f - xi_1);
        float r = std::sqrt(xi_1), phi = global::kTwoPi * xi_2;
        float sin_phi = std::sin(phi), cos_phi = std::cos(phi);
        global::Vector local(r * cos_phi, r * sin_phi, z);
        return ToWorld(local, normal);
    }
    return global::kNone;
}

float Lambert::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    float cos_theta = normal.dot(wi);
    if (normal.dot(wo) > 0 && cos_theta > 0) {
        return cos_theta * global::kInvPi;
    }
    return 0.f;
}

std::ostream &operator<<(std::ostream &os, const Lambert &lambert) {
    using namespace global;
    os << "Lambert:\n\tkd = " << lambert.albedo_;
    if (lambert.HasEmitter()) {
        os << *((Material *) &lambert);
    }
    return os;
}
