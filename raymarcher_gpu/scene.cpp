//
//  scene.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#include "scene.h"

bool Scene::init(THREAD_ADDR_SPACE Object* objects, size_t num_objs) {
    if (num_objs > kMaxObjects) {
        return false;
    }
    
    std::memcpy(objects_, objects, num_objs * sizeof(Object));
    num_objs_ = num_objs;
    return true;
}
