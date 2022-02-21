//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_PATH_TRACER_H
#define PATH_TRACER_PATH_TRACER_H


#include "../camera/camera.h"
#include "../geometry/scene.h"
#include "../ray/ray.h"
#include "../utils/funcs.h"

#include "ThreadPool/ThreadPool.h"

#include <eigen3/Eigen/Core>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>

class PathTracer {
public:
    PathTracer(const Camera &camera, const Scene &scene);

    void Render(int spp);

    void Save(const std::string &filename);

private:
    Camera camera_;
    Scene scene_;
    std::vector<Eigen::Vector3f> fragment_buffer_;

};


#endif //PATH_TRACER_PATH_TRACER_H
