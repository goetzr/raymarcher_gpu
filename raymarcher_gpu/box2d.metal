//
//  box2d.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/20/26.
//

#include "box2d.h"

#include <metal_stdlib>
using namespace metal;

#include "shared.h"

FLOAT Box2D::sdf(THREAD_ADDR_SPACE const FLOAT3& p) const {
    FLOAT3 p2 = world_to_local_.rotation * p + world_to_local_.translation;

    // 1. Calculate 2D half-extents.
    FLOAT bx = size_.width * 0.5f;
    FLOAT by = size_.height * 0.5f;

    // 2. Component-wise distance in the 2D plane (XY).
    FLOAT dx = abs(p2.x) - bx;
    FLOAT dy = abs(p2.y) - by;

    // 3. Distance to the 2D rectangle boundary within the plane.
    FLOAT d2d_outside = sqrt(
        max(dx, 0.0f) * max(dx, 0.0f) +
        max(dy, 0.0f) * max(dy, 0.0f)
    );
    FLOAT d2d_inside = min(max(dx, dy), 0.0f);
    FLOAT d2d = d2d_outside + d2d_inside;

    // 4. Combine with the Z distance (distance to the plane).
    if (d2d > 0.0f) {
        // Point is outside the rectangle's footprint.
        return sqrt(d2d * d2d + p2.z * p2.z);
    } else {
        // Point is directly above/below the rectangle's area
        return abs(p2.z);
    }
}
