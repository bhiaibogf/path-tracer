//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_RAY_H
#define PATH_TRACER_RAY_H


#include "../utils/global.h"

#include <eigen3/Eigen/Eigen>

class Ray {
public:
    Ray(const Eigen::Vector3f &origin, const Eigen::Vector3f &direction);

    ~Ray() = default;

    auto origin() const { return origin_; }

    auto direction() const { return direction_; }

    bool Update(float t);

    Eigen::Vector3f operator()(float t) const;

    friend std::ostream &operator<<(std::ostream &os, const Ray &ray);

private:
    static const float kEpsilon;

    Eigen::Vector3f origin_, direction_;
    float t_min_, t_max_;

};


#endif //PATH_TRACER_RAY_H
