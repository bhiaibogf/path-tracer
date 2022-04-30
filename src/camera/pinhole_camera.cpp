//
// Created by bhiaibogf on 2022/4/14.
//

#include "pinhole_camera.h"


PinholeCamera::PinholeCamera(const Eigen::Vector3f &eye, const Eigen::Vector3f &look_at, const Eigen::Vector3f &up,
                             float fov_y, int width, int height) : Camera(eye, look_at, up, fov_y, width, height) {}

Ray PinholeCamera::GenerateRay(int x, int y, bool antialiasing) const {
    auto uv = SampleFilm(x, y, antialiasing);
    global::Vector dir = uv.x() * right_ + uv.y() * up_ + direction_;
    return {eye_, dir.normalized()};
}
