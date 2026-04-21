//
//  cube.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "shared.h"

class Cube {
public:
    Cube(THREAD_ADDR_SPACE FLOAT3 center, FLOAT side_len, FLOAT3 rotation) noexcept;
    FLOAT sdf(THREAD_ADDR_SPACE const FLOAT3& p) const;

private:
    // The cube's center, in world coordinates.
    FLOAT3 center_;
    // The side length of a face of the cube.
    FLOAT side_len_;
    // The cube's basis vectors, in world coordinates.
    FLOAT3x3 basis_{};
    // The 4x4 world to local matrix.
    CoordTransform world_to_local_{};
};
