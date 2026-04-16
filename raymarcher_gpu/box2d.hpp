//
//  box2d.hpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "vec3.hpp"
#include "common.hpp"

class Box2D  {
public:
    Box2D(const Vec3& center, RectF size, EulerAngles rotation) noexcept;
    FLOAT sdf(const Vec3& p) const noexcept;

private:
    // The box's center, in world coordinates.
    Vec3 center_;
    RectF size_;
    // The box's basis vectors, in world coordinates.
    Mat3 basis_;
    // The 4x4 world to local matrix.
    CoordTransform world_to_local_;
};
