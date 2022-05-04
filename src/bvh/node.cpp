//
// Created by bhiaibogf on 2022/2/25.
//

#include "node.h"

#include <thread-pool/thread_pool.hpp>

const std::size_t Node::kMaxPrimitives = 8;
const std::size_t Node::kUseSah = 1024;
const int Node::kDepth = 4;

Node::Node(std::vector<const Primitive *> *primitives, SplitMethod split_method, int depth) {
    Bound centroid_bound;
    for (auto primitive: *primitives) {
        centroid_bound += primitive->bound().Centroid();
    }

    if (primitives->size() <= kMaxPrimitives || centroid_bound.IsPoint()) {
        bound_ = Bound();
        area_weighted_ = 0.f;
        left_ = nullptr;
        right_ = nullptr;

        for (auto primitive: *primitives) {
            primitives_.push_back(primitive);
            bound_ |= primitive->bound();
            area_weighted_ += primitive->AreaWeighted();
        }
        return;
    } else {
        int dim = centroid_bound.MaxExtent();

        int split_pos = -1;
        switch (split_method) {
            case kMix:
            case kMiddle:
                split_pos = std::partition(primitives->begin(), primitives->end(),
                                           [dim, centroid_bound](const Primitive *primitive) {
                                               return primitive->bound().Centroid()[dim]
                                                      < centroid_bound.Centroid()[dim];
                                           }) - primitives->begin();
                break;
            case kEqualCount:
                std::nth_element(primitives->begin(), primitives->begin() + primitives->size() / 2,
                                 primitives->end(),
                                 [dim](const Primitive *primitive_1, const Primitive *primitive_2) {
                                     return primitive_1->bound().Centroid()[dim]
                                            < primitive_2->bound().Centroid()[dim];
                                 });
                split_pos = primitives->size() / 2;
                break;
            case kSah:
                std::sort(primitives->begin(), primitives->end(), [dim](auto primitive_1, auto primitive_2) {
                    return primitive_1->bound().Centroid()[dim] < primitive_2->bound().Centroid()[dim];
                });

                float min_cost = global::kInf;
                for (int i = 1; i < primitives->size(); i++) {
                    float cost = 0.f;
                    Bound left_bound, right_bound;
                    for (int j = 0; j < i; j++) {
                        left_bound |= primitives->at(j)->bound();
                    }
                    cost += float(i) * left_bound.SurfaceArea();
                    for (int j = i; j < primitives->size(); j++) {
                        right_bound |= primitives->at(j)->bound();
                    }
                    cost += float(primitives->size() - i) * right_bound.SurfaceArea();
                    if (cost < min_cost) {
                        min_cost = cost;
                        split_pos = i;
                    }
                }
        }

        auto beginning = primitives->begin();
        auto middling = primitives->begin() + split_pos;
        auto ending = primitives->end();

        auto left = new std::vector<const Primitive *>(beginning, middling);
        auto right = new std::vector<const Primitive *>(middling, ending);

        assert(primitives->size() == (left->size() + right->size()));

        if (depth < kDepth) {
            thread_pool pool(std::thread::hardware_concurrency());
            std::future<Node *> left_future, right_future;
            if (split_method == kMix) {
                left_future = pool.submit([&]() {
                    return new Node(left, left->size() < kUseSah ? kSah : kMix, depth + 1);
                });
                right_future = pool.submit([&]() {
                    return new Node(right, right->size() < kUseSah ? kSah : kMix, depth + 1);
                });
            } else {
                left_future = pool.submit([&]() {
                    return new Node(left, split_method, depth + 1);
                });
                right_future = pool.submit([&]() {
                    return new Node(right, split_method, depth + 1);
                });
            }
            left_ = left_future.get();
            right_ = right_future.get();
        } else {
            if (split_method == kMix) {
                left_ = new Node(left, left->size() < kUseSah ? kSah : kMix, depth + 1);
                right_ = new Node(right, right->size() < kUseSah ? kSah : kMix, depth + 1);
            } else {
                left_ = new Node(left, split_method, depth + 1);
                right_ = new Node(right, split_method, depth + 1);
            }
        }

        bound_ = left_->bound_ | right_->bound_;
        area_weighted_ = left_->area_weighted_ + right_->area_weighted_;
    }
}

bool Node::Intersect(Ray *ray, Intersection *intersection) const {
    if (!primitives_.empty()) {
        bool has_intersection = false;
        for (auto primitive: primitives_) {
            if (primitive->Intersect(ray, intersection)) {
                has_intersection = true;
            }
        }
        return has_intersection;
    }

    // calculate the distance from the bounding box center to the light source
    auto left_dis = left_->bound_.Centroid() - ray->origin(),
            right_dis = right_->bound_.Centroid() - ray->origin();
    bool left_is_nearer = left_dis.squaredNorm() < right_dis.squaredNorm();

    // handle closer aabb first
    auto near = left_is_nearer ? left_ : right_,
            far = left_is_nearer ? right_ : left_;

    bool has_intersection = false;
    if (near->bound_.Intersect(*ray)) {
        if (near->Intersect(ray, intersection)) {
            has_intersection = true;
        }
    }

    if (far->bound_.Intersect(*ray)) {
        if (far->Intersect(ray, intersection)) {
            has_intersection = true;
        }
    }

    return has_intersection;
}

void Node::SampleLight(Intersection *intersection, float *pdf, float area) const {
    if (!primitives_.empty()) {
        float area_sum = 0.f;
        for (auto primitive: primitives_) {
            area_sum += primitive->AreaWeighted();
            if (area <= area_sum) {
                primitive->Sample(intersection, pdf);
                return;
            }
        }
    }

    if (area <= left_->area_weighted_) {
        left_->SampleLight(intersection, pdf, area);
    } else {
        right_->SampleLight(intersection, pdf, area - left_->area_weighted_);
    }
}
