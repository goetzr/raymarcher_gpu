//
//  cube_gpu.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "cube.h"

FLOAT sdf(constant const Cube& cube, thread const FLOAT3& p);
