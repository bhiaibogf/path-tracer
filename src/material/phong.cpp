//
// Created by bhiaibogf on 2022/1/23.
//

#include "phong.h"

void Phong::Prepare(const global::TexCoord &tex_coord) {
    k_s_.Prepare(tex_coord);
    n_s_.Prepare(tex_coord);
}

global::Color Phong::Albedo() const {
    return k_s_.value();
}

global::Color Phong::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    if (normal.dot(wo) > 0 && normal.dot(wi) > 0) {
        global::Vector reflect = global::Reflect(wo, normal);
        float alpha = wi.dot(reflect);
        if (alpha > 0) {
            return k_s_.value() * (n_s_.value() + 2) * global::kInvTwoPi * std::pow(alpha, n_s_.value());
        }
    }
    return global::kBlack;
}

global::Vector Phong::Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const {
    if (normal.dot(wo) > 0) {
        global::Vector reflect = global::Reflect(wo, normal);

        auto xi_1 = generator::Rand(), xi_2 = generator::Rand();

        float z = std::pow(xi_1, 1.f / (n_s_.value() + 1.f));
        float r = std::sqrt(1.f - z * z);

        float phi = global::kTwoPi * xi_2;
        float sin_phi = std::sin(phi), cos_phi = std::cos(phi);

        global::Vector local(r * cos_phi, r * sin_phi, z);
        *pdf = (n_s_.value() + 1) * global::kInvTwoPi * std::pow(z, n_s_.value());
        return global::ToWorld(local, reflect);
    }
    *pdf = 0.f;
    return global::kNone;
}

float Phong::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    if (normal.dot(wo) > 0) {
        global::Vector reflect = global::Reflect(wo, normal);
        float alpha = wi.dot(reflect);
        if (alpha > 0) {
            return (n_s_.value() + 1) * global::kInvTwoPi * std::pow(alpha, n_s_.value());
        }
    }
    return 0.f;
}

std::ostream &operator<<(std::ostream &os, const Phong &phong) {
    os << "Phong:\n\tks = " << phong.k_s_ << "\n\tns = " << phong.n_s_;
    if (phong.HasEmitter()) {
        os << *((Material *) &phong);
    }
    return os;
}
