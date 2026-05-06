//
//  shared.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/11/26.
//

#pragma once

#ifdef __METAL_VERSION__
    #include <metal_stdlib>
    using namespace metal;
    using FLOAT = float;
    using FLOAT3 = float3;
    using FLOAT3x3 = float3x3;
#else
    #include <simd/simd.h>
    using FLOAT = float;
    using FLOAT3 = simd::float3;
    using FLOAT3x3 = simd::float3x3;
#endif

struct RectF {
    FLOAT width;
    FLOAT height;
};

struct RectI {
    uint32_t width;
    uint32_t height;
};

struct Fov {
    FLOAT horiz;
    FLOAT vert;
};

enum class SensorFit {
    // Decrease the canvas size (FOV) to match the output aspect ratio.
    Fill,
    // Increase the canvas size (FOV) to match the output aspect ratio.
    Overscan
};

// A 4x4 coordinate transformation matrix, split out into its rotation matrix and translation vector.
struct CoordTransform {
    FLOAT3x3 rotation;
    FLOAT3 translation;
};

struct ClosestObject {
    int index;
    FLOAT distance;
};
