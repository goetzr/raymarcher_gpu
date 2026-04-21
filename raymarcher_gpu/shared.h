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
    #define THREAD_ADDR_SPACE thread
    #define CONST_ADDR_SPACE constant
#else
    #include <simd/simd.h>
    using FLOAT = float;
    using FLOAT3 = simd::float3;
    using FLOAT3x3 = simd::float3x3;
    #define THREAD_ADDR_SPACE
    #define CONST_ADDR_SPACE 
#endif

//#ifdef __METAL_VERSION__
//
//    inline FLOAT length(FLOAT3 v) noexcept {
//        return metal::length(v);
//    }
//
//    inline FLOAT3 normalize(FLOAT3 v) noexcept {
//        return metal::normalize(v);
//    }
//
//    inline FLOAT dot(FLOAT3 a, FLOAT3 b) noexcept {
//        return metal::dot(a, b);
//    }
//
//    inline FLOAT3x3 transpose(FLOAT3x3 m) noexcept {
//        return metal::transpose(m);
//    }
//
//    inline float determinant(FLOAT3x3 m) noexcept {
//        return metal::determinant(m);
//    }
//
//    inline FLOAT3x3 inverse(FLOAT3x3 m) {
//        return metal::inverse(m);
//    }
//
//#else // __METAL_VERSION__
//
//    inline FLOAT length(FLOAT3 v) noexcept {
//        return simd_length(v);
//    }
//
//    inline FLOAT3 normalize(FLOAT3 v) noexcept {
//        return simd_normalize(v);
//    }
//
//    inline FLOAT dot(FLOAT3 a, FLOAT3 b) noexcept {
//        return simd_dot(a, b);
//    }
//
//    inline FLOAT3x3 transpose(FLOAT3x3 m) noexcept {
//        return simd_transpose(m);
//    }
//
//    inline float determinant(FLOAT3x3 m) noexcept {
//        return simd_determinant(m);
//    }
//
//    inline FLOAT3x3 inverse(FLOAT3x3 m) {
//        return simd_inverse(m);
//    }
//
//#endif

struct RectF {
    FLOAT width;
    FLOAT height;
};

struct RectI {
    int32_t width;
    int32_t height;
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
    double distance;
};
