//
//  sphere.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/20/26.
//

#include "sphere.h"

Sphere::Sphere(const FLOAT3& center, FLOAT radius) noexcept
    : center_{center}, radius_{radius}
{}
