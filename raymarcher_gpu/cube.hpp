//
//  cube.hpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "vec3.hpp"
#include "shared.h"
#include "mat3.hpp"

class Cube {
public:
    Cube(Vec3 center, FLOAT side_len, EulerAngles rotation) noexcept;
    FLOAT sdf(const Vec3& p) const noexcept;

private:
    // The cube's center, in world coordinates.
    Vec3 center_;
    // The side length of a face of the cube.
    FLOAT side_len_;
    // The cube's basis vectors, in world coordinates.
    Mat3 basis_;
    // The 4x4 world to local matrix.
    CoordTransform world_to_local_;
};
