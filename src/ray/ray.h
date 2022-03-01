//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_RAY_H
#define PATH_TRACER_RAY_H


#include "../utils/global.h"

class Ray {
public:
    Ray(const global::Vector &origin, const global::Vector &direction);

    ~Ray() = default;

    auto origin() const { return origin_; }

    auto direction() const { return direction_; }

    auto t_min() const { return t_min_; }

    auto t_max() const { return t_max_; }

    auto t() const { return t_max_; }

    bool Update(float t);

    friend std::ostream &operator<<(std::ostream &os, const Ray &ray);

private:
    static const float kEpsilon;

    global::Vector origin_, direction_;
    float t_min_, t_max_;

    global::Vector operator()(float t) const;

};


#endif //PATH_TRACER_RAY_H
