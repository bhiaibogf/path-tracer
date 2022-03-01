//
// Created by bhiaibogf on 2022/1/23.
//

#include "camera.h"

Camera::Camera(global::Vector eye, const global::Vector &look_at, const global::Vector &up,
               float fov, int width, int height)
        : eye_(std::move(eye)), fov_(fov), width_(width), height_(height) {
    direction_ = (look_at - eye_).normalized();
    right_ = direction_.cross(up).normalized();
    up_ = right_.cross(direction_).normalized();
}

int Camera::GetIndex(int x, int y) const {
    assert(x >= 0 && x < width_);
    assert(y >= 0 && y < height_);
    return (height_ - y - 1) * width_ + x;
}

Ray Camera::GenerateRay(int x, int y, bool antialiasing) const {
    float tangent = std::tan(global::Radius(fov_) / 2.f);
    auto width = float(width_), height = float(height_), aspect = width / height;

    float xx = float(x) + 0.5f, yy = float(y) + 0.5f;
    if (antialiasing) {
        xx += generator::RandN();
        yy += generator::RandN();
    }

    float x_factor = tangent * aspect * (xx - width / 2.f) / (width / 2.f);
    float y_factor = tangent * (yy - height / 2.f) / (height / 2.f);

    auto direction_x = x_factor * right_;
    auto direction_y = y_factor * up_;
    auto direction_z = direction_;

    global::Vector direction_final = direction_x + direction_y + direction_z;
    direction_final.normalize();

    return {eye_, direction_final};
}
