//
//  sphere.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/20/26.
//

#include "sphere.h"

FLOAT Sphere::sdf(THREAD_ADDR_SPACE const FLOAT3& p) const constant {
    return length(p - center_) - radius_;
}
