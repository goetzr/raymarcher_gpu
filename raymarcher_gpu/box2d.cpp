//
//  box2d.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#include "box2d.h"
#include "util.h"

Box2D::Box2D(const FLOAT3& center, RectF size, FLOAT3 rotation) noexcept
        : center_{center}, size_{size}
{
    // Calculate the basis vectors from the rotation.
    rotation.x = deg2rad(rotation.x);
    rotation.y = deg2rad(rotation.y);
    rotation.z = deg2rad(rotation.z);
    basis_ = generate_basis(rotation);

    // Calculate the world to local matrix from the basis vectors and the center position.
    world_to_local_ = inverse_coord_transform(basis_, center_);
}
