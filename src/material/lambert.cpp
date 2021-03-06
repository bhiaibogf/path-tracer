//
// Created by bhiaibogf on 2022/2/22.
//

#include "lambert.h"

template
Lambert::Lambert(const std::string &k_d);

template
Lambert::Lambert(const global::Color &k_d);

template<class T>
Lambert::Lambert(const T &k_d) : albedo_(k_d) {}

void Lambert::Prepare(const global::TexCoord &tex_coord) {
    albedo_.Prepare(tex_coord);
}

global::Color Lambert::Albedo() const {
    return albedo_.value();
}

global::Color Lambert::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    if (normal.dot(wo) > 0 && normal.dot(wi) > 0) {
        return albedo_.value() * global::kInvPi;
    }
    return global::kBlack;
}

global::Vector Lambert::Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const {
    if (normal.dot(wo) > 0) {
        auto xi_1 = generator::Rand(), xi_2 = generator::Rand();
        float z = std::sqrt(1.f - xi_1);
        float r = std::sqrt(xi_1), phi = global::kTwoPi * xi_2;
        float sin_phi = std::sin(phi), cos_phi = std::cos(phi);
        global::Vector local(r * cos_phi, r * sin_phi, z);
        *pdf = z * global::kInvPi;
        return global::ToWorld(local, normal);
    }
    *pdf = 0.f;
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
    os << "Lambert:\n\tkd = " << lambert.albedo_;
    if (lambert.HasEmitter()) {
        os << *((Material *) &lambert);
    }
    return os;
}
