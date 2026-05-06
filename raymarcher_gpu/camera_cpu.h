//
//  camera_cpu.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 5/1/26.
//

#pragma once

#include "camera.h"

void init(Camera& camera, FLOAT3 pos, FLOAT3 rotation,
          FLOAT fov_horiz, FLOAT sensor_aspect_ratio, FLOAT output_aspect_ratio, SensorFit sensor_fit,
          FLOAT clip_near, FLOAT clip_far) noexcept;

void adjust_fov(Camera& camera, FLOAT sensor_aspect_ratio, FLOAT output_aspect_ratio, SensorFit sensor_fit) noexcept;
