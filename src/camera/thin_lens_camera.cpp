//
// Created by bhiaibogf on 2022/4/14.
//

#include "thin_lens_camera.h"

ThinLensCamera::ThinLensCamera(const Eigen::Vector3f &eye, const Eigen::Vector3f &look_at, const Eigen::Vector3f &up,
                               float fov, int width, int height) : Camera(eye, look_at, up, fov, width, height) {
    focal_length_ = 26.f;
    lens_radius_ = 0.1f;
    fov_y_ = 2 * global::Degree(std::atan(35.f / aspect_ / 2.f / focal_length_));
}

Ray ThinLensCamera::GenerateRay(int x, int y, bool antialiasing) const {
    auto uv = SampleFilm(x, y, antialiasing);
    auto direction_camera = global::Vector(uv.x(), uv.y(), -1.f).normalized();

    // Sample point on lens
    global::Vector2 p_lens = lens_radius_ * generator::SampleDisk();
    auto origin = eye_ + p_lens.x() * right_ + p_lens.y() * up_;

    // Compute point on plane of focus
    float time = focal_length_ / -direction_camera.z();
    global::Vector p_focus = eye_ + time * (uv.x() * right_ + uv.y() * up_ + direction_).normalized();

    auto direction = (p_focus - origin).normalized();

    return {origin, direction};
}
