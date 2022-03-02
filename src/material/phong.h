//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_PHONG_H
#define PATH_TRACER_PHONG_H


#include "material.h"

class Phong : public Material {
public:
    Phong(const global::Color &k_s, float n_s);

    MaterialType Type() const override { return MaterialType::kPhong; }

    global::Color Albedo() const override;

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    global::Vector Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const override;

    float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    friend std::ostream &operator<<(std::ostream &os, const Phong &phong);

private:
    global::Color k_s_;
    float n_s_;

};


#endif //PATH_TRACER_PHONG_H
