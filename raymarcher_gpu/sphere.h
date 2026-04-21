//
//  sphere.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "shared.h"

class Sphere  {
public:
    inline Sphere(THREAD_ADDR_SPACE const FLOAT3& center, FLOAT radius) noexcept;
    inline FLOAT sdf(THREAD_ADDR_SPACE const FLOAT3& p) const;

private:
    FLOAT3 center_;
    FLOAT radius_;
};
