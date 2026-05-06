//
//  camera.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/14/26.
//

#pragma once

#include "shared.h"

struct Camera {
    // The camera's position, expressed in world coordinates.
    FLOAT3 pos{};
    
    // The camera's local coordinate system is a right-handed x/y/z system where:
    //   i_hat points right along the x-axis,
    //   j_hat points up along the y-axis,
    //   k_hat points backward along the z-axis, opposite the camera's forward direction.
    // These basis vectors are expressed in world coordinates.
    FLOAT3x3 basis{};

    // The 4x4 world to camera matrix.
    CoordTransform world_to_camera{};

    // How to adjust the canvas size (FOV) to match the output aspect ratio.
    // SensorFit sensor_fit_;
    // The FOV, in radians.
    Fov fov{};

    // The near and far clipping planes are imaginary planes located at specific distances from the camera
    // along the camera's line of sight.
    // Only objects located between the camera's two clipping planes are rendered in the camera's view.
    //
    // The distance, in world units, to the near clipping plane.
    FLOAT clip_near = 0.0f;
    // The distance, in world units, to the far clipping plane.
    FLOAT clip_far = 0.0f;
};
