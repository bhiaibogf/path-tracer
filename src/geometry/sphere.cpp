//
// Created by bhiaibogf on 2022/3/4.
//

#include "sphere.h"

Sphere::Sphere(Object *object, global::Vector center, float radius)
        : Primitive(object), center_(std::move(center)), radius_(radius) {
    area_ = global::kPi * radius_ * radius_;
    bound_ = Bound(center_ + global::Vector(-radius_, -radius_, -radius_),
                   center_ + global::Vector(radius_, radius_, radius_));
}

// 6.837
bool Sphere::Intersect(Ray *ray, Intersection *intersection) const {
    global::Vector r_0 = ray->origin() - center_, r_d = ray->direction();
    bool outside = (r_0.norm() >= radius_);

    float t_p = -r_0.dot(r_d);
    if (outside && t_p < 0) {
        return false;
    }

    float sqr_d = std::pow(r_0.norm(), 2.f) - std::pow(t_p, 2.f), sqr_r = std::pow(radius_, 2.f);
    if (sqr_d > sqr_r) {
        return false;
    }

    float t_prime = std::sqrt(sqr_r - sqr_d);
    float t = outside ? t_p - t_prime : t_p + t_prime;
    if (ray->Update(t)) {
        intersection->position = (*ray)(t);
        intersection->normal = (intersection->position - center_).normalized();
        intersection->tex_coord = global::kNoTex;
        intersection->material = material();
        intersection->weight = Weight();
        return true;
    }

    return false;
}

void Sphere::Sample(Intersection *intersection, float *pdf) const {
    float xi_1 = generator::Rand(), xi_2 = generator::Rand();
    float z = 1 - 2 * xi_1, r = std::sqrt(1.f - z * z);
    float phi = global::kTwoPi * xi_2;
    float x = r * std::cos(phi), y = r * std::sin(phi);

    intersection->normal = {x, y, z};
    intersection->position = center_ + intersection->normal * radius_;

    intersection->weight = Weight();
    *pdf = Weight();
}

void Sphere::InsertTo(std::vector<const Primitive *> *primitives) const {
    primitives->push_back(this);
}

std::ostream &operator<<(std::ostream &os, const Sphere &sphere) {
    using namespace global;
    os << "Sphere:\n\t" << "center : " << sphere.center_ << "\n\tradius = " << sphere.radius_;
    return os;
}
