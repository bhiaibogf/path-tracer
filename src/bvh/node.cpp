//
// Created by bhiaibogf on 2022/2/25.
//

#include "node.h"

const int Node::kMaxPrimitives = 8;

Node::Node(std::vector<const Primitive *> *primitives) {
    if (primitives->size() <= kMaxPrimitives) {
        for (auto primitive: *primitives) {
            primitives_.push_back(primitive);
            bound_ |= primitive->bound();
            area_ += primitive->AreaWeighted();
        }
        left_ = nullptr;
        right_ = nullptr;
        return;
    } else {
        Bound centroid_bound;
        for (auto primitive: *primitives) {
            centroid_bound += primitive->bound().Centroid();
        }

        int dim = centroid_bound.MaxExtent();
        std::sort(primitives->begin(), primitives->end(), [&](auto primitive_1, auto primitive_2) {
            return primitive_1->bound().Centroid()[dim] < primitive_2->bound().Centroid()[dim];
        });

        auto beginning = primitives->begin();
        auto middling = primitives->begin() + int(primitives->size()) / 2;
        auto ending = primitives->end();

        auto left = new std::vector<const Primitive *>(beginning, middling);
        auto right = new std::vector<const Primitive *>(middling, ending);

        assert(primitives->size() == (left->size() + right->size()));

        left_ = new Node(left);
        right_ = new Node(right);
    }
    bound_ = left_->bound_ | right_->bound_;
    area_ = left_->area_ + right_->area_;
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

    // 计算包围盒中心到光源的距离
    auto left_dis = left_->bound_.Centroid() - ray->origin(),
            right_dis = right_->bound_.Centroid() - ray->origin();
    bool left_is_nearer = left_dis.squaredNorm() < right_dis.squaredNorm();

    // 由于场景过于空旷，上面的计算可能反而带来很大的开销
    // bool flag = true;

    // 优先判断离光源近的包围盒
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

bool Node::SampleLight(Intersection *intersection, float *pdf, float area) const {
    if (!primitives_.empty()) {
        float area_sum = 0.f;
        for (auto primitive: primitives_) {
            area_sum += primitive->AreaWeighted();
            if (area <= area_sum) {
                primitive->Sample(intersection, pdf);
                return true;
            }
        }
        return false;
    }

    // TODO EPS
    if (area <= left_->area_) {
        if (left_->SampleLight(intersection, pdf, area)) {
            return true;
        } else {
            return right_->SampleLight(intersection, pdf, area - left_->area_);
        }
    } else {
        if (right_->SampleLight(intersection, pdf, area - left_->area_)) {
            return true;
        } else {
            return left_->SampleLight(intersection, pdf, area);
        }
    }
}
