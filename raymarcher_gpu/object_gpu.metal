//
//  object_gpu.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/20/26.
//

#include "object_gpu.h"
#include "sphere_gpu.h"
#include "cube_gpu.h"
#include "box2d_gpu.h"

constant constexpr FLOAT kMaxDistance = 10000;

FLOAT sdf(constant const Object& obj, thread const FLOAT3& p) {
    switch (obj.type) {
        case ObjectType::Unused:
            return kMaxDistance;
        case ObjectType::Sphere:
            return sdf(obj.sphere, p);
        case ObjectType::Cube:
            return sdf(obj.cube, p);
        case ObjectType::Box2D:
            return sdf(obj.box2d, p);
        default:
            return kMaxDistance;
    }
}
