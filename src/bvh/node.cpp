//
// Created by bhiaibogf on 2022/2/25.
//

#include "node.h"

Node::Node(std::vector<Primitive *> primitives) {
    if (primitives.size() == 1) {
        bound = primitives[0]->bound();
        primitive = primitives[0];
        area = primitive->area();

        left = nullptr;
        right = nullptr;
        return;
    } else if (primitives.size() == 2) {
        left = new Node(std::vector{primitives[0]});
        right = new Node(std::vector{primitives[1]});
    } else {
        Bound centroid_bound;
        for (auto primitive: primitives) {
            centroid_bound += primitive->bound().Centroid();
        }

        int dim = centroid_bound.MaxExtent();
        std::sort(primitives.begin(), primitives.end(), [&](auto f1, auto f2) {
            return f1->bound().Centroid()[dim] < f2->bound().Centroid()[dim];
        });


        auto beginning = primitives.begin();
        auto middling = primitives.begin() + (primitives.size() / 2);
        auto ending = primitives.end();

        auto leftshapes = std::vector<Primitive *>(beginning, middling);
        auto rightshapes = std::vector<Primitive *>(middling, ending);

        assert(primitives.size() == (leftshapes.size() + rightshapes.size()));

        left = new Node(leftshapes);
        right = new Node(rightshapes);
    }
    bound = left->bound | right->bound;
    area = left->area + right->area;
}

bool Node::Intersect(Ray *ray, Intersection *intersection) const {
    if (primitive) {
        return primitive->Intersect(ray, intersection);
    }

    // 计算包围盒中心到光源的距离
    auto left_dis = left->bound.Centroid() - ray->origin(),
            right_dis = right->bound.Centroid() - ray->origin();
    bool flag = left_dis.squaredNorm() < right_dis.squaredNorm();

    // 由于场景过于空旷，上面的计算可能反而带来很大的开销
    // bool flag = true;

    // 优先判断离光源近的包围盒
    auto near = flag ? left : right,
            far = flag ? right : left;

    bool has_intersection = false;
    if (near->bound.Intersect(*ray)) {
        if (near->Intersect(ray, intersection)) {
            has_intersection = true;
        }
    }

    if (far->bound.Intersect(*ray)) {
        if (far->Intersect(ray, intersection)) {
            has_intersection = true;
        }
    }

    return has_intersection;
}
