//
//  core_types.hpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/11/26.
//

#pragma once

#include "core_types.h"

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
