//
// Created by bhiaibogf on 2022/2/25.
//

#include "node.h"

Node::Node(std::vector<Primitive *> *primitives) {
    if (primitives->size() == 1) {
        bound_ = (*primitives)[0]->bound();
        primitive_ = (*primitives)[0];
        area_ = primitive_->material()->HasEmitter() ? primitive_->area() : 0.f;

        left_ = nullptr;
        right_ = nullptr;
        return;
    } else if (primitives->size() == 2) {
        left_ = new Node(new std::vector{(*primitives)[0]});
        right_ = new Node(new std::vector{(*primitives)[1]});
    } else {
        Bound centroid_bound;
        for (auto primitive: *primitives) {
            centroid_bound += primitive->bound().Centroid();
        }

        int dim = centroid_bound.MaxExtent();
        std::sort(primitives->begin(), primitives->end(), [&](auto f1, auto f2) {
            return f1->bound().Centroid()[dim] < f2->bound().Centroid()[dim];
        });


        auto beginning = primitives->begin();
        auto middling = primitives->begin() + (primitives->size() / 2);
        auto ending = primitives->end();

        auto leftshapes = new std::vector<Primitive *>(beginning, middling);
        auto rightshapes = new std::vector<Primitive *>(middling, ending);

        assert(primitives->size() == (leftshapes->size() + rightshapes->size()));

        left_ = new Node(leftshapes);
        right_ = new Node(rightshapes);
    }
    primitive_ = nullptr;
    bound_ = left_->bound_ | right_->bound_;
    area_ = left_->area_ + right_->area_;
}

bool Node::Intersect(Ray *ray, Intersection *intersection) const {
    if (primitive_) {
        return primitive_->Intersect(ray, intersection);
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
    if (primitive_) {
        if (primitive_->material()->HasEmitter()) {
            assert(area_ <= primitive_->area());
            primitive_->Sample(intersection, pdf);
            return true;
        } else {
            return false;
        }
    } else {
        if (area <= left_->area_) {
            if (left_->SampleLight(intersection, pdf, area)) {
                return true;
            }
        }
        return right_->SampleLight(intersection, pdf, area - left_->area_);
    }
}
