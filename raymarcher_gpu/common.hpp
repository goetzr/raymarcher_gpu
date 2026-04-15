//
//  shared.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/11/26.
//

#pragma once

#ifdef __METAL_VERSION__
    using FLOAT = float;
    using FLOAT3 = float3;
    using FLOAT3x3 = float3x3;
    #define SH_ADDR_SPACE device
#else
    #include <simd/simd.h>
    using FLOAT = float;
    using FLOAT3 = simd::float3;
    using FLOAT3x3 = simd::float3x3;
    #define SH_ADDR_SPACE
#endif

struct RectF {
    FLOAT width;
    FLOAT height;
};

struct RectI {
    int32_t width;
    int32_t height;
};

struct EulerAngles {
    FLOAT x;
    FLOAT y;
    FLOAT z;
};

// A 4x4 coordinate transformation matrix, split out into its rotation matrix and translation vector.
struct CoordTransform {
    Mat3 rotation;
    Vec3 translation;
};
