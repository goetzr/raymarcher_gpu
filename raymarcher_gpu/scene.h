//
//  scene.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "object.h"

CONST_ADDR_SPACE constexpr int kMaxObjects = 100;

class Scene {
public:
    bool init(THREAD_ADDR_SPACE Object* objects, size_t num_objs);
    bool closest_object(THREAD_ADDR_SPACE const FLOAT3& p,
                        THREAD_ADDR_SPACE ClosestObject& closest_obj) const CONST_ADDR_SPACE;

private:
    Object objects_[kMaxObjects];
    size_t num_objs_ = 0;
};
