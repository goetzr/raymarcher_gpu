//
//  sphere.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/20/26.
//

#include "sphere.h"

#include <metal_stdlib>
using namespace metal;

FLOAT Sphere::sdf(THREAD_ADDR_SPACE const FLOAT3& p) const {
    return length(p - center_) - radius_;
}
