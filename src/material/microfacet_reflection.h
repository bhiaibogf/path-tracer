//
// Created by bhiaibogf on 2022/4/13.
//

#ifndef PATH_TRACER_MICROFACET_REFLECTION_H
#define PATH_TRACER_MICROFACET_REFLECTION_H


#include "material.h"

class MicrofacetReflection : public Material {
public:
    template<class T>
    explicit MicrofacetReflection(const T &alpha):alpha_{alpha}, albedo_{global::kWhite}, eta_{1.5f} {}

    template<class T1, class T2>
    explicit MicrofacetReflection(const T1 &alpha, const T2 &albedo, float eta = 1.5f)
            :alpha_{alpha}, albedo_{albedo}, eta_{eta} {}

    ~MicrofacetReflection() override = default;

    MaterialType Type() const override { return kMicrofacet; }

    void Prepare(const global::TexCoord &tex_coord) override;

    global::Color Albedo() const override;

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    global::Vector Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const override;

    float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

protected:
    Parameter<global::Color> albedo_;

    Parameter<float> alpha_;
    float eta_;

    float Fresnel(const global::Vector &wi, const global::Vector &normal) const;

    float CookTorranceFresnel(const global::Vector &wi, const global::Vector &normal) const;

    float Geometry(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const;

    virtual float Geometry(float cos_theta) const;

    virtual float Distribution(float cos_theta) const;

};


#endif //PATH_TRACER_MICROFACET_REFLECTION_H
