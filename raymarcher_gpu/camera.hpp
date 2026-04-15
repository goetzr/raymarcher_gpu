//
//  camera.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/14/26.
//

#pragma once

#include "shared.h"

enum class SensorFit {
    // Decrease the canvas size (FOV) to match the output aspect ratio.
    Fill,
    // Increase the canvas size (FOV) to match the output aspect ratio.
    Overscan
};

struct Fov {
    FLOAT horiz;
    FLOAT vert;
};

class Camera {
public:
    Camera(FLOAT sensor_aspect_ratio, FLOAT fov_horiz, SensorFit sensor_fit, FLOAT clip_near, FLOAT clip_far,
               FLOAT3 pos, EulerAngles rotation) noexcept;

    Fov fov() const noexcept { return fov_; }
    FLOAT sensor_aspect_ratio() const noexcept { return sensor_aspect_ratio_; }
    SensorFit sensor_fit() const noexcept { return sensor_fit_; }
    FLOAT3x3 basis() const noexcept { return basis_; }
    FLOAT3 position() const noexcept { return pos_; }
    const CoordTransform& world_to_camera() const noexcept { return world_to_camera_; }
    FLOAT clip_near() const noexcept { return clip_near_; }
    FLOAT clip_far() const noexcept { return clip_far_; }
    
private:
    // The focal length and sensor size define the **internal** geometry of the camera.
    // Together they determine the FOV.
    // NOTE: The focal length and sensor size are unnecessary for a raymarcher.
    //       The desired sensor aspect ratio and FOV are specified directly.
    //
    // The focal length, in mm.
    // This is the distance between the aperture (pinhole) and the image plane (film) in a physical pinhole camera.
    // double focal_len_;
    // The sensor size, in inches.
    // This is the film size in an analog camera, or the sensor size in a digital camera.
    // It defines the sensor aspect ratio.
    // RectF sensor_sz_;

    // The ratio of the width to the height of the film/sensor.
    FLOAT sensor_aspect_ratio_;
    // The FOV, in radians.
    Fov fov_;
    // How to adjust the canvas size (FOV) to match the output aspect ratio.
    SensorFit sensor_fit_;

    // The near and far clipping planes are imaginary planes located at specific distances from the camera
    // along the camera's line of sight.
    // Only objects located between the camera's two clipping planes are rendered in the camera's view.
    //
    // The distance, in world units, to the near clipping plane.
    FLOAT clip_near_;
    // The distance, in world units, to the far clipping plane.
    FLOAT clip_far_;

    // The camera's position, expressed in world coordinates.
    FLOAT3 pos_;

    // The camera's local coordinate system is a right-handed x/y/z system where:
    //   i_hat points right along the x-axis,
    //   j_hat points up along the y-axis,
    //   k_hat points backward along the z-axis, opposite the camera's forward direction.
    // These basis vectors are expressed in world coordinates.
    FLOAT3x3 basis_;

    // The size, in world units, of the canvas.
    // NOTE: This is unnecessary for a raymarcher.
    // RectF canvas_sz_;

    // The 4x4 world to camera matrix.
    CoordTransform world_to_camera_;
};
