//
// Created by bhiaibogf on 2022/1/23.
//

#include "camera.h"

Camera::Camera(global::Vector eye, const global::Vector &look_at, const global::Vector &up,
               float fov_y, int width, int height)
        : eye_(std::move(eye)), fov_y_(fov_y), width_(width), height_(height) {
    width_f_ = static_cast<float>(width_);
    height_f_ = static_cast<float>(height_);
    aspect_ = width_f_ / height_f_;

    direction_ = (look_at - eye_).normalized();
    right_ = direction_.cross(up).normalized();
    up_ = right_.cross(direction_).normalized();
}

int Camera::GetIndex(int x, int y) const {
    assert(x >= 0 && x < width_);
    assert(y >= 0 && y < height_);
    return (height_ - y - 1) * width_ + x;
}

global::Vector2 Camera::SampleFilm(int x, int y, bool antialiasing) const {
    float tangent = std::tan(global::Radius(fov_y_) / 2.f);

    float xx = float(x) + 0.5f, yy = float(y) + 0.5f;
    if (antialiasing) {
        xx += generator::RandN();
        yy += generator::RandN();
    }

    float x_factor = tangent * aspect_ * (xx - width_f_ / 2.f) / (width_f_ / 2.f);
    float y_factor = tangent * (yy - height_f_ / 2.f) / (height_f_ / 2.f);

    return {x_factor, y_factor};
}

std::ostream &operator<<(std::ostream &os, const Camera &camera) {
    auto [x, y, z] = std::make_tuple(camera.direction_.x(), camera.direction_.y(), camera.direction_.z());

    float cos_x = std::sqrt((x * x + z * z) / (x * x + y * y + z * z));
    if (z < 0) cos_x = -cos_x;
    float cos_y = z / std::sqrt(x * x + z * z);

    float fov_x = global::Degree(2.f * std::atan(camera.aspect_ * std::tan(global::Radius(camera.fov_y_ / 2.f))));

    using namespace global;
    std::cout << "Camera:\n  position: " << global::Vector{camera.eye_.x(), -camera.eye_.z(), camera.eye_.y()}
              << "\n  direction: " << camera.direction_
              << "\n  xyz Euler: [" << global::Degree(std::acos(cos_x)) - 90 <<
              ", 0, " << 180 - global::Degree(std::acos(cos_y)) << "]"
              << "\n  fov: " << std::max(camera.fov_y_, fov_x)
              << "\n  size: " << camera.width_ << " * " << camera.height_;
    return os;
}
