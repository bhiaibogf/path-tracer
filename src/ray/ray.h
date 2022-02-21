//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_RAY_H
#define PATH_TRACER_RAY_H


#include <eigen3/Eigen/Eigen>

class Ray {
public:
    Ray(const Eigen::Vector3f &origin, const Eigen::Vector3f &direction);

    ~Ray() = default;

    auto direction() const { return direction_; }

    Eigen::Vector3f operator()(float t) const;

    friend std::ostream &operator<<(std::ostream &os, const Ray &ray);

private:
    Eigen::Vector3f origin_, direction_;
    float t_, t_min_, t_max_;

};


#endif //PATH_TRACER_RAY_H
