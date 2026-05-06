//
//  scene_cpu.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#include "scene_cpu.h"

bool init(Scene& scene, Object* objects, size_t num_objs) noexcept {
    if (num_objs > MAX_OBJECTS) {
        return false;
    }
    
    std::memcpy(scene.objects, objects, num_objs * sizeof(Object));
    scene.num_objs = num_objs;
    return true;
}
