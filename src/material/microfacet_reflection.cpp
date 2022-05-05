//
// Created by bhiaibogf on 2022/4/13.
//

#include "microfacet_reflection.h"

void MicrofacetReflection::Prepare(const global::TexCoord &tex_coord) {
    alpha_.Prepare(tex_coord);
    albedo_.Prepare(tex_coord);
}

global::Color MicrofacetReflection::Albedo() const {
    return albedo_.value();
}

global::Color
MicrofacetReflection::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    auto half = (wo + wi).normalized();

    float d = Distribution(normal.dot(half));
    float g = Geometry(wo, wi, normal);
    float f = Fresnel(wi, half);

    return albedo_.value() * d * g * f / (4 * normal.dot(wo) * normal.dot(wi));
}

global::Vector MicrofacetReflection::Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const {
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

float
MicrofacetReflection::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    float cos_theta = normal.dot(wi);
    if (normal.dot(wo) > 0 && cos_theta > 0) {
        return cos_theta * global::kInvPi;
    }
    return 0.f;
}

float MicrofacetReflection::Fresnel(const global::Vector &wi, const global::Vector &normal) const {
    return global::Schlick(wi, normal, eta_);
}

float MicrofacetReflection::CookTorranceFresnel(const global::Vector &wi, const global::Vector &normal) const {
    auto c = normal.dot(wi);
    auto g = global::Sqr(eta_) + global::Sqr(c) - 1.f;
    if (g > 0) {
        g = std::sqrt(g);
        auto a = (g - c) / (g + c),
                b = (c * (g + c) - 1) / (c * (g - c) + 1);
        return 0.5f * global::Sqr(a) * (1 + global::Sqr(b));
    }
    return 1.f;
}

float
MicrofacetReflection::Geometry(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return Geometry(normal.dot(wo)) * Geometry(normal.dot(wi));
}

float MicrofacetReflection::Geometry(float cos_theta) const {
    return 1.f;
}

float MicrofacetReflection::Distribution(float cos_theta) const {
    return 1.f;
}
