//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_CAMERA_H
#define PATH_TRACER_CAMERA_H


#include "../ray/ray.h"
#include "../utils/global.h"

#include <eigen3/Eigen/Eigen>

class Camera {
public:
    Camera(const Eigen::Vector3f &eye, const Eigen::Vector3f &look_at, const Eigen::Vector3f &up,
           float fov, int width, int height);

    ~Camera() = default;

    auto width() const { return width_; }

    auto height() const { return height_; }

    int GetIndex(int x, int y) const;

    Ray GenerateRay(int x, int y) const;

private:
    Eigen::Vector3f eye_, look_at_, up_;
    float fov_;
    int width_, height_;

};


#endif //PATH_TRACER_CAMERA_H
