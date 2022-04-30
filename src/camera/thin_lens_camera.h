//
// Created by bhiaibogf on 2022/4/14.
//

#ifndef PATH_TRACER_THIN_LENS_CAMERA_H
#define PATH_TRACER_THIN_LENS_CAMERA_H


#include "camera.h"

class ThinLensCamera : public Camera {
public:
    ThinLensCamera(const Eigen::Vector3f &eye, const Eigen::Vector3f &look_at, const Eigen::Vector3f &up, float fov,
                   int width, int height);

    ~ThinLensCamera() override = default;

    Ray GenerateRay(int x, int y, bool antialiasing) const override;

private:
    float lens_radius_;
    float focal_length_;

};


#endif //PATH_TRACER_THIN_LENS_CAMERA_H
