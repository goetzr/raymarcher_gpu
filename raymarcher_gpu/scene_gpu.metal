//
//  scene.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/20/26.
//

#include "scene.h"

bool Scene::closest_object(THREAD_ADDR_SPACE const FLOAT3& p,
                           THREAD_ADDR_SPACE ClosestObject& closest_obj) const constant {
    if (num_objs_ == 0) {
        return false;
    }

    ClosestObject co {
        .index = 0,
        .distance = objects_[0].sdf(p)
    };

    for (size_t i = 1; i != num_objs_; ++i) {
        FLOAT dist = objects_[i].sdf(p);
        if (dist < co.distance) {
            co.index = i;
            co.distance = dist;
        }
    }

    closest_obj = co;
    return true;
}
