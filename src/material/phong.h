//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_PHONG_H
#define PATH_TRACER_PHONG_H


#include "material.h"

class Phong : public Material {
public:
    bool IsEmitter() const override;

    global::Color emission() const override;

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    global::Vector Sample(const global::Vector &wo, const global::Vector &normal) const override;

    float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

private:

};


#endif //PATH_TRACER_PHONG_H
