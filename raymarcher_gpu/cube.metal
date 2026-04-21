//
//  cube.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/20/26.
//

#include "cube.h"

#include <metal_stdlib>
using namespace metal;

FLOAT Cube::sdf(THREAD_ADDR_SPACE const FLOAT3& p) const {
    // Transform the point from world coordinates to local coordinates.
    FLOAT3 p2 = world_to_local_.rotation * p + world_to_local_.translation;

    // 1. Calculate half-extent.
    FLOAT b = side_len_ * 0.5f;

    // 2. Component-wise distance to the faces.
    FLOAT dx = abs(p2.x) - b;
    FLOAT dy = abs(p2.y) - b;
    FLOAT dz = abs(p2.z) - b;

    // 3. Distance to the outside of the cube.
    FLOAT d2d_outside = sqrt(
        max(dx, 0.0f) * max(dx, 0.0f) +
        max(dy, 0.0f) * max(dy, 0.0f) +
        max(dz, 0.0f) * max(dz, 0.0f)
    );

    // Distance when inside the box (nearest face).
    FLOAT d2d_inside = min(max(dx, max(dy, dz)), 0.0f);

    return d2d_outside + d2d_inside;
}
