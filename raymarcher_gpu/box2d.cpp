//
//  box2d.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#include "box2d.h"
#include "util.h"

Box2D::Box2D(const Vec3& center, RectF size, FLOAT3 rotation) noexcept
        : center_{center}, size_{size}, basis_{}, world_to_local_{}
{
    // Calculate the basis vectors from the rotation.
    rotation.x = deg2rad(rotation.x);
    rotation.y = deg2rad(rotation.y);
    rotation.z = deg2rad(rotation.z);
    basis_ = generate_basis(rotation);

    // Calculate the world to local matrix from the basis vectors and the center position.
    world_to_local_ = inverse_coord_transform(basis_, center_);
}

FLOAT Box2D::sdf(const Vec3& p) const noexcept {
    Vec3 p2 = world_to_local_.rotation * p + world_to_local_.translation;

    // 1. Calculate 2D half-extents.
    FLOAT bx = size_.width * 0.5f;
    FLOAT by = size_.height * 0.5f;

    // 2. Component-wise distance in the 2D plane (XY).
    FLOAT dx = std::abs(p2.x) - bx;
    FLOAT dy = std::abs(p2.y) - by;

    // 3. Distance to the 2D rectangle boundary within the plane.
    FLOAT d2d_outside = std::sqrt(
        std::max(dx, 0.0f) * std::max(dx, 0.0f) +
        std::max(dy, 0.0f) * std::max(dy, 0.0f)
    );
    FLOAT d2d_inside = std::min(std::max(dx, dy), 0.0f);
    FLOAT d2d = d2d_outside + d2d_inside;

    // 4. Combine with the Z distance (distance to the plane).
    if (d2d > 0.0f) {
        // Point is outside the rectangle's footprint.
        return std::sqrt(d2d * d2d + p2.z * p2.z);
    } else {
        // Point is directly above/below the rectangle's area
        return std::abs(p2.z);
    }
}
