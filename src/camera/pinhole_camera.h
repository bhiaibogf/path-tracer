//
// Created by bhiaibogf on 2022/4/14.
//

#ifndef PATH_TRACER_PINHOLE_CAMERA_H
#define PATH_TRACER_PINHOLE_CAMERA_H


#include "camera.h"

class PinholeCamera : public Camera {
public:
    PinholeCamera(const Eigen::Vector3f &eye, const Eigen::Vector3f &look_at, const Eigen::Vector3f &up, float fov_y,
                  int width, int height);

    ~PinholeCamera() override = default;

    Ray GenerateRay(int x, int y, bool antialiasing) const override;

};


#endif //PATH_TRACER_PINHOLE_CAMERA_H
