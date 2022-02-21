//
// Created by bhiaibogf on 2022/1/23.
//

#include "path_tracer.h"

PathTracer::PathTracer(const Camera &camera, const Scene &scene) : camera_(camera), scene_(scene) {
    fragment_buffer_.resize(camera_.width() * camera_.height());
    std::fill(fragment_buffer_.begin(), fragment_buffer_.end(), Eigen::Vector3f(0, 0, 0));
}

void PathTracer::Render(int spp) {
    ThreadPool pool(std::thread::hardware_concurrency());
    for (int i = 0; i < camera_.height(); i++) {
        std::future<Eigen::Vector3f> futures[camera_.width()][spp];
        for (int j = 0; j < camera_.width(); j++) {
            for (int k = 0; k < spp; k++) {
                futures[j][k] = pool.enqueue([&]() {
                    return scene_.Trace(camera_.GenerateRay(i, j), 0);
                });
            }
        }
        for (int j = 0; j < camera_.width(); j++) {
            for (int k = 0; k < spp; k++) {
                fragment_buffer_[camera_.GetIndex(i, j)] += futures[j][k].get() / spp;
            }
        }
        global::UpdateProgress(i / camera_.height());
    }
    global::UpdateProgress(1.f);

    for (auto &pixel: fragment_buffer_) {
        pixel /= float(spp);
    }
}

void PathTracer::Save(const std::string &filename) {
    auto fragment_mat_ = new cv::Mat(camera_.height(), camera_.width(), CV_32FC3, fragment_buffer_.data());
    cv::cvtColor(*fragment_mat_, *fragment_mat_, cv::COLOR_BGR2RGB);
    // fragment_mat_->convertTo(*fragment_mat_, CV_8UC3, 255.0f);
    cv::imwrite(filename, *fragment_mat_);
}
