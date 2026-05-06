//
//  sphere_cpu.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/20/26.
//

#include "sphere_cpu.h"

void init(Sphere& sphere, const FLOAT3& center, FLOAT radius) noexcept {
    sphere.center = center;
    sphere.radius = radius;
}
