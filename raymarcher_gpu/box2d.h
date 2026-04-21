//
//  box2d.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "shared.h"

class Box2D  {
public:
    Box2D(THREAD_ADDR_SPACE const FLOAT3& center, RectF size, FLOAT3 rotation) noexcept;
    FLOAT sdf(THREAD_ADDR_SPACE const FLOAT3& p) const;

private:
    // The box's center, in world coordinates.
    FLOAT3 center_;
    RectF size_;
    // The box's basis vectors, in world coordinates.
    FLOAT3x3 basis_{};
    // The 4x4 world to local matrix.
    CoordTransform world_to_local_{};
};
