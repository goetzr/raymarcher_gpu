//
//  object.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#include "object.hpp"

FLOAT Object::sdf(const Vec3& p) const noexcept {
    static constexpr FLOAT kMaxDistance = 10000;

    switch (type_) {
        case ObjectType::Unused:
            return kMaxDistance;
        case ObjectType::Sphere:
            return sphere_.sdf(p);
        case ObjectType::Cube:
            return cube_.sdf(p);
        case ObjectType::Box2D:
            return box2d_.sdf(p);
        default:
            return kMaxDistance;
    }
}
