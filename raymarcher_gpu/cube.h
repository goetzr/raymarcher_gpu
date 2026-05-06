//
//  cube.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "shared.h"

struct Cube {
    // The cube's center, in world coordinates.
    FLOAT3 center{};
    // The side length of a face of the cube.
    FLOAT side_len = 0.0f;
    // The cube's basis vectors, in world coordinates.
    FLOAT3x3 basis{};
    // The 4x4 world to local matrix.
    CoordTransform world_to_local{};
};
