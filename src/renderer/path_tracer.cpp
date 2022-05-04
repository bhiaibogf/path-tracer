//
// Created by bhiaibogf on 2022/1/23.
//

#include "path_tracer.h"

PathTracer::PathTracer(const Camera *camera, const Scene *scene) : camera_(camera), scene_(scene) {
    fragment_buffer_.resize(camera_->width() * camera_->height());
    std::fill(fragment_buffer_.begin(), fragment_buffer_.end(), Eigen::Vector3f(0, 0, 0));
}

void PathTracer::Render(int spp, bool antialiasing, Scene::ShadingType shading_type) {
    // ThreadPool pool(std::thread::hardware_concurrency());
    for (int y = 0; y < camera_->height(); y++) {
        // std::future<Eigen::Vector3f> futures[camera_.width()][spp];
#pragma omp parallel for default(none) shared(fragment_buffer_, y, camera_, spp, antialiasing, shading_type)
        for (int x = 0; x < camera_->width(); x++) {
            for (int k = 0; k < spp; k++) {
                Ray ray = camera_->GenerateRay(x, y, antialiasing);
                fragment_buffer_[camera_->GetIndex(x, y)] += scene_->Trace(&ray, shading_type);
                // futures[x][k] = pool.enqueue([&]() {
                //     return scene_.Trace(&ray);
                // });
            }
        }
        // for (int x = 0; x < camera_.width(); x++) {
        //     for (int k = 0; k < spp; k++) {
        //         fragment_buffer_[camera_.GetIndex(x, y)] += futures[x][k].get() / spp;
        //     }
        // }
        global::UpdateProgress(float(y) / camera_->height());
    }
    global::UpdateProgress(1.f);

    for (auto &pixel: fragment_buffer_) {
        pixel /= float(spp);
    }
}

void PathTracer::Save(const std::string &filename) {
    auto *buffer = new float[3 * camera_->width() * camera_->height()];
    for (int i = 0; i < camera_->width() * camera_->height(); i++) {
        buffer[3 * i + 0] = fragment_buffer_[i].x();
        buffer[3 * i + 1] = fragment_buffer_[i].y();
        buffer[3 * i + 2] = fragment_buffer_[i].z();
    }
    SaveEXR(buffer, camera_->width(), camera_->height(), 3, 0, filename.c_str(), nullptr);
    std::cout << "\nImage saved to ./" << filename << std::endl;
}
