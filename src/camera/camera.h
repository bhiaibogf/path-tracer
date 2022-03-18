//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_CAMERA_H
#define PATH_TRACER_CAMERA_H


#include "../utils/generator.h"
#include "../ray/ray.h"

class Camera {
public:
    Camera(Eigen::Vector3f eye, const Eigen::Vector3f &look_at, const Eigen::Vector3f &up,
           float fov, int width, int height);

    ~Camera() = default;

    auto width() const { return width_; }

    auto height() const { return height_; }

    int GetIndex(int x, int y) const;

    Ray GenerateRay(int x, int y, bool antialiasing) const;

    friend std::ostream &operator<<(std::ostream &os, const Camera &camera);

private:
    global::Vector eye_, direction_, right_, up_;
    float fov_y_;
    int width_, height_;

};


#endif //PATH_TRACER_CAMERA_H
