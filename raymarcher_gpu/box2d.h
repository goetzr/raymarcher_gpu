//
//  box2d.hpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "core_types.h"
#include "common.h"

class Box2D  {
public:
    Box2D(const FLOAT3& center, RectF size, FLOAT3 rotation) noexcept;
    FLOAT sdf(const FLOAT3& p) const noexcept;

private:
    // The box's center, in world coordinates.
    FLOAT3 center_;
    RectF size_;
    // The box's basis vectors, in world coordinates.
    FLOAT3x3 basis_;
    // The 4x4 world to local matrix.
    CoordTransform world_to_local_;
};
