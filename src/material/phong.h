//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_PHONG_H
#define PATH_TRACER_PHONG_H


#include "material.h"

class Phong : public Material {
public:
    template<class T1, class T2>
    Phong(T1 k_s, T2 n_s):k_s_(Parameter<global::Color>(k_s)), n_s_(Parameter<float>(n_s)) {}

    ~Phong() override = default;

    MaterialType Type() const override { return MaterialType::kPhong; }

    global::Color Albedo() const override;

    global::Color Albedo(const global::TexCoord &tex_coord) const override;

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal,
                       const global::TexCoord &tex_coord) const override;

    global::Vector Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const override;

    float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    friend std::ostream &operator<<(std::ostream &os, const Phong &phong);

private:
    Parameter<global::Color> k_s_;
    Parameter<float> n_s_;

};


#endif //PATH_TRACER_PHONG_H
