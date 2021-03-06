//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_PHONG_H
#define PATH_TRACER_PHONG_H


#include "material.h"

class Phong : public Material {
public:
    template<class T1, class T2>
    Phong(T1 k_s, T2 n_s):k_s_(k_s), n_s_(n_s) {}

    ~Phong() override = default;

    MaterialType Type() const override { return MaterialType::kPhong; }

    void Prepare(const global::TexCoord &tex_coord) override;

    global::Color Albedo() const override;

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    global::Vector Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const override;

    float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    friend std::ostream &operator<<(std::ostream &os, const Phong &phong);

private:
    Parameter<global::Color> k_s_;
    Parameter<float> n_s_;

};


#endif //PATH_TRACER_PHONG_H
