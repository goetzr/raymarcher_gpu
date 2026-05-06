//
//  box2d_gpu.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 5/2/26.
//

#include "box2d.h"

FLOAT sdf(constant const Box2D& box, thread const FLOAT3& p);
