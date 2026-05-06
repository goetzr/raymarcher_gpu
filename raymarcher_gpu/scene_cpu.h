//
//  scene_cpu.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "scene.h"

bool init(Scene& scene, Object* objects, size_t num_objs) noexcept;
