//
//  scene.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "object.h"

#define MAX_OBJECTS 100
struct Scene {
    Object objects[MAX_OBJECTS] = {};
    size_t num_objs = 0;
};
