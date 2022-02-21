//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_MATERIAL_H
#define PATH_TRACER_MATERIAL_H


#include <eigen3/Eigen/Eigen>

class Material {
public:
    virtual bool IsEmitter() const { return false; }

    virtual Eigen::Vector3f emission() const { assert(false); }

    virtual Eigen::Vector3f
    Eval(const Eigen::Vector3f &wo, const Eigen::Vector3f &wi, const Eigen::Vector3f &normal) const = 0;

    virtual Eigen::Vector3f Sample(const Eigen::Vector3f &wo, const Eigen::Vector3f &normal) const = 0;

    virtual float Pdf(const Eigen::Vector3f &wo, const Eigen::Vector3f &wi, const Eigen::Vector3f &normal) const = 0;

private:

};


#endif //PATH_TRACER_MATERIAL_H
