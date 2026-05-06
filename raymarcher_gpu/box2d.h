//
//  box2d.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "shared.h"

struct Box2D  {
    // The box's center, in world coordinates.
    FLOAT3 center{};
    RectF size{};
    // The box's basis vectors, in world coordinates.
    FLOAT3x3 basis{};
    // The 4x4 world to local matrix.
    CoordTransform world_to_local{};
};
