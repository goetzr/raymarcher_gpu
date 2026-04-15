//
//  sphere.hpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "vec3.hpp"

class Sphere  {
public:
    Sphere(const Vec3& center, FLOAT radius) noexcept
        : center_{center}, radius_{radius}
    {}

    FLOAT sdf(const Vec3& p) const noexcept {
        return (p - center_).length() - radius_;
    }

private:
    Vec3 center_;
    FLOAT radius_;
};
