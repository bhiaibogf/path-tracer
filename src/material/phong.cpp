//
// Created by bhiaibogf on 2022/1/23.
//

#include "phong.h"

global::Color Phong::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    if (normal.dot(wo) > 0 && normal.dot(wi) > 0) {
        global::Vector reflect = global::Reflect(wo, normal);
        float alpha = wi.dot(reflect);
        if (alpha > 0) {
            return k_s_.GetValue() * (n_s_.GetValue() + 2) * global::kInvTwoPi * std::pow(alpha, n_s_.GetValue());
        }
    }
    return global::kBlack;
}

global::Color Phong::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal,
                          const global::TexCoord &tex_coord) const {
    if (normal.dot(wo) > 0 && normal.dot(wi) > 0) {
        global::Vector reflect = global::Reflect(wo, normal);
        float alpha = wi.dot(reflect);
        if (alpha > 0) {
            return k_s_.GetValue(tex_coord) * (n_s_.GetValue(tex_coord) + 2) * global::kInvTwoPi *
                   std::pow(alpha, n_s_.GetValue(tex_coord));
        }
    }
    return global::kBlack;
}

global::Vector Phong::Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const {
    if (normal.dot(wo) > 0) {
        global::Vector reflect = global::Reflect(wo, normal);

        auto xi_1 = generator::Rand(), xi_2 = generator::Rand();

        float z = std::pow(xi_1, 1.f / (n_s_.GetValue() + 1.f));
        float r = std::sqrt(1.f - z * z);

        float phi = global::kTwoPi * xi_2;
        float sin_phi = std::sin(phi), cos_phi = std::cos(phi);

        global::Vector local(r * cos_phi, r * sin_phi, z);
        *pdf = (n_s_.GetValue() + 1) * global::kInvTwoPi * std::pow(z, n_s_.GetValue());
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
            return (n_s_.GetValue() + 1) * global::kInvTwoPi * std::pow(alpha, n_s_.GetValue());
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

global::Color Phong::Albedo() const {
    return k_s_.GetValue();
}

global::Color Phong::Albedo(const global::TexCoord &tex_coord) const {
    return k_s_.GetValue(tex_coord);
}
