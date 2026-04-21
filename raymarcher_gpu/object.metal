//
//  object.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/20/26.
//

#include "object.h"

#include <metal_stdlib>
using namespace metal;

constant constexpr FLOAT kMaxDistance = 10000;

FLOAT Object::sdf(THREAD_ADDR_SPACE const FLOAT3& p) const {
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
