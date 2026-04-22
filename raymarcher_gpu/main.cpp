//
//  main.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/5/26.
//

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "shared.h"
#include "camera.h"
#include "object.h"
#include "scene.h"
#include "raymarcher.h"

constexpr RectI kOutputSize = {
    .width = 1024,
    .height = 768
};
constexpr FLOAT kOutputAspectRatio = (FLOAT)kOutputSize.width / kOutputSize.height;

int main(int argc, const char * argv[]) {
    // Create the camera.
    FLOAT3 cam_pos { 0, 0, 0 };
    FLOAT3 cam_rot { 0, 0, 0 };
    FLOAT fov_horiz = 80;
    FLOAT sensor_aspect_ratio = 3.0 / 2;
    FLOAT clip_near = 0.1;
    FLOAT clip_far = 100;
    Camera camera {
        cam_pos, cam_rot,
        fov_horiz, sensor_aspect_ratio, kOutputAspectRatio, SensorFit::Overscan,
        clip_near, clip_far
    };
    
    // Create the scene to render.
    // NOTE: The camera is facing along the negative z-axis.
    //       If an object is to be visible, its z coordinate must be negative.
    Object scene_objects[] = {
        Sphere {
            FLOAT3 { -15, 0, -30 },
            5
        },
        Cube {
            FLOAT3 { 0, 0, -30 },
            10,
            FLOAT3 { 0, 45, 0 }
        },
        Box2D {
            FLOAT3 { 15, 0, -30 },
            { 10, 10 },
            FLOAT3 { 0, 45, 0 }
        }
    };
    size_t num_objs = sizeof(scene_objects) / sizeof(Object);
    Scene scene;
    if (!scene.init(scene_objects, num_objs)) {
        std::cerr << "ERROR: failed to initialize the scene to render\n";
        return 1;
    }
    
    Raymarcher marcher;
    std::string marcher_error;
    if (!marcher.init(marcher_error)) {
        std::cerr << "ERROR: failed to initialize the ray marcher: " << marcher_error << "\n";
        return 1;
    }
}
