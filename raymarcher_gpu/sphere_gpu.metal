//
//  sphere_gpu.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/20/26.
//

#include "sphere_gpu.h"

FLOAT sdf(constant const Sphere& sphere, thread const FLOAT3& p) {
    return length(p - sphere.center) - sphere.radius;
}
