//
//  cube_cpu.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#include "cube_cpu.h"
#include "util.h"

void init(Cube& cube, FLOAT3 center, FLOAT side_len, FLOAT3 rotation) noexcept
{
    cube.center = center;
    cube.side_len = side_len;
    
    // Calculate the basis vectors from the rotation.
    rotation.x = deg2rad(rotation.x);
    rotation.y = deg2rad(rotation.y);
    rotation.z = deg2rad(rotation.z);
    cube.basis = generate_basis(rotation);

    // Calculate the world to local matrix from the basis vectors and the center position.
    cube.world_to_local = inverse_coord_transform(cube.basis, cube.center);
}
