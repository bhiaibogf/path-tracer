//
// Created by bhiaibogf on 2022/2/22.
//

#ifndef PATH_TRACER_LAMBERT_H
#define PATH_TRACER_LAMBERT_H

#include "material.h"

class Lambert : public Material {
public:
    explicit Lambert(const global::Color &k_d);

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    global::Vector Sample(const global::Vector &wo, const global::Vector &normal) const override;

    float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    friend std::ostream &operator<<(std::ostream &os, const Lambert &lambert);

private:
    global::Color albedo_;

};


#endif //PATH_TRACER_LAMBERT_H
