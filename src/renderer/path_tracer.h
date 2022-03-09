//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_PATH_TRACER_H
#define PATH_TRACER_PATH_TRACER_H


#include "../camera/camera.h"
#include "../scene/scene.h"

#include <opencv2/opencv.hpp>

class PathTracer {
public:
    PathTracer(const Camera *camera, const Scene *scene);

    void Render(int spp, bool antialiasing, Scene::SampleType sample_type);

    void Save(const std::string &filename);

private:
    const Camera *camera_;
    const Scene *scene_;
    std::vector<Eigen::Vector3f> fragment_buffer_;

};


#endif //PATH_TRACER_PATH_TRACER_H
