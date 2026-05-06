//
//  box2d_cpu.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 5/2/26.
//

#pragma once

#include "box2d.h"

void init(Box2D& box2d, const FLOAT3& center, RectF size, FLOAT3 rotation) noexcept;
