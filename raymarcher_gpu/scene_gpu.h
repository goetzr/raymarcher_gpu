//
//  scene_gpu.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "scene.h"

bool closest_object(constant const Scene& scene, thread const FLOAT3& p,
                    thread ClosestObject& closest_obj);
