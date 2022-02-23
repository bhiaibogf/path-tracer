//
// Created by bhiaibogf on 2022/1/23.
//

#include "camera.h"

Camera::Camera(const Eigen::Vector3f &eye, const Eigen::Vector3f &look_at, const Eigen::Vector3f &up,
               float fov, int width, int height)
        : eye_(eye), look_at_(look_at), up_(up), fov_(fov), width_(width), height_(height) {
}

int Camera::GetIndex(int x, int y) const {
    assert(x >= 0 && x < width_);
    assert(y >= 0 && y < height_);
    return (height_ - y - 1) * width_ + x;
}

Ray Camera::GenerateRay(int x, int y, bool antialiasing) const {
    Eigen::Vector3f direction = look_at_ - eye_;
    direction.normalize();
    Eigen::Vector3f right = direction.cross(up_);
    right.normalize();
    Eigen::Vector3f up = right.cross(direction);
    up.normalize();

    float tangent = std::tan(global::Radius(fov_) / 2.0f);
    auto width = float(width_), height = float(height_), aspect = width / height;

    float xx = float(x) + 0.5f, yy = float(y) + 0.5f;
    if (antialiasing) {
        xx += global::RandN();
        yy += global::RandN();
    }
    float x_factor = tangent * aspect * (xx - width / 2.f) / (width / 2.f);
    float y_factor = tangent * (yy - height / 2.f) / (height / 2.f);

    Eigen::Vector3f direction_x = x_factor * right;
    Eigen::Vector3f direction_y = y_factor * up;
    Eigen::Vector3f direction_z = direction;

    Eigen::Vector3f direction_final = direction_x + direction_y + direction_z;
    direction_final.normalize();

    return {eye_, direction_final};
}
