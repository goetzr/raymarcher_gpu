//
//  scene_gpu.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/20/26.
//

#include "scene_gpu.h"
#include "object_gpu.h"

bool closest_object(constant const Scene& scene, thread const FLOAT3& p,
                    thread ClosestObject& closest_obj) {
    if (scene.num_objs == 0) {
        return false;
    }

    ClosestObject co {
        .index = 0,
        .distance = sdf(scene.objects[0], p)
    };

    for (size_t i = 1; i != scene.num_objs; ++i) {
        FLOAT dist = sdf(scene.objects[i], p);
        if (dist < co.distance) {
            co.index = i;
            co.distance = dist;
        }
    }

    closest_obj = co;
    return true;
}
