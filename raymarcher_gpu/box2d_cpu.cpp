//
//  box2d_cpu.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#include "box2d_cpu.h"
#include "util.h"

void init(Box2D& box, const FLOAT3& center, RectF size, FLOAT3 rotation) noexcept {
    box.center = center;
    box.size = size;
    
    // Calculate the basis vectors from the rotation.
    rotation.x = deg2rad(rotation.x);
    rotation.y = deg2rad(rotation.y);
    rotation.z = deg2rad(rotation.z);
    box.basis = generate_basis(rotation);

    // Calculate the world to local matrix from the basis vectors and the center position.
    box.world_to_local = inverse_coord_transform(box.basis, box.center);
}
