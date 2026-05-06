//
//  object_gpu.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "object.h"

FLOAT sdf(constant const Object& obj, thread const FLOAT3& p);
