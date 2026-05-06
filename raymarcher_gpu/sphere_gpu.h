//
//  sphere_gpu.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "sphere.h"

FLOAT sdf(constant const Sphere& sphere, thread const FLOAT3& p);
