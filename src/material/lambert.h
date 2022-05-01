//
// Created by bhiaibogf on 2022/2/22.
//

#ifndef PATH_TRACER_LAMBERT_H
#define PATH_TRACER_LAMBERT_H


#include "material.h"

class Lambert : public Material {
public:
    template<class T>
    explicit Lambert(const T &k_d);

    ~Lambert() override = default;

    MaterialType Type() const override { return MaterialType::kLambert; }

    global::Color Albedo() const override;

    global::Color Albedo(const global::TexCoord &tex_coord) const override;

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal,
                       const global::TexCoord &tex_coord) const override;

    global::Vector Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const override;

    float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    friend std::ostream &operator<<(std::ostream &os, const Lambert &lambert);

private:
    Parameter<global::Color> albedo_;

};


#endif //PATH_TRACER_LAMBERT_H
