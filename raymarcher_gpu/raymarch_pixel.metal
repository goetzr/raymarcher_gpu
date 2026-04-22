//
//  raymarch_pixel.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/11/26.
//

#include <metal_stdlib>
using namespace metal;

#include "camera.h"
#include "shared.h"
#include "scene.h"

// A ray that has marched further than this distance is defined to have missed all objects in the scene.
constant static constexpr float kMaxDist = 200;
// A ray that has been marched this many steps is defined to have missed all objects in the scene.
constant static constexpr int kMaxSteps = 50;
// A ray has hit an object in the scene if its distance to that object is less than Epsilon.
constant static constexpr float kEpsilon = 0.001;

kernel void raymarch_pixel(constant const RectI& output_sz [[buffer(0)]],
                           constant const Camera& camera [[buffer(1)]],
                           constant const Scene& scene [[buffer(2)]],
                           device uint32_t* pixels [[buffer(3)]],
                           uint2 gid [[thread_position_in_grid]])
{
    
    
    // NOTE: The raster is sampled from the portion of the scene visible on the virtual canvas.
    
    // NOTE: Graphics APIs follow the convention used in the calculations that follow.
    //       scratchapixel.com follows a different convention:
    //         pipeline: raster space -> NDC space -> screen space, where
    //         NDC space has its origin at the bottom-left and coordinates are in the range [0, 1]
    //         screen space has its origin at the center and coordinates are in the range [-1, 1]
    //       Both conventions arrive at the same result.
    
    // Check for out-of-bounds if the raster dimensions aren't a multiple of threadgroup size.
    if (gid.x >= output_sz.width || gid.y >= output_sz.height) {
        return;
    }
    
    // Convert from raster coordinates to Normalized Screen Coordinates (NSC).
    // NSC space has its origin at the top-left and coordinates are in the range [0, 1].
    float u = (gid.x + 0.5f) / output_sz.width;
    float v = (gid.y + 0.5f) / output_sz.height;
    
    // Convert from NSC to Normalized Device Coordinates (NDC).
    // NDC space has its origin at the center and coordinates are in the range [-1, 1].
    float x_ndc = 2 * u - 1;
    float y_ndc = 1 - 2 * v;
    
    // Apply the NDC coordinates to the FOV to get a ray direction vector in camera space.
    // Assume the virtual canvas is 1 unit away from the camera along the z-axis (simplifies tan).
    float x_cam = x_ndc * tan(camera.fov().horiz * 0.5f);
    float y_cam = y_ndc * tan(camera.fov().vert  * 0.5f);
    float z_cam = -1;
    
    // Normalize the ray direction vector.
    FLOAT3 ray_dir_cam = normalize(FLOAT3 { x_cam, y_cam, z_cam });
    
    // Convert the ray direction vector into world coordinates.
    FLOAT3 ray_dir = camera.basis() * ray_dir_cam;
    
    // Use the distance to the closest object as the initial ray length.
    FLOAT3 current_position = camera.position();
    ClosestObject closest_obj = {
        .index = -1,
        .distance = 10'000
    };
    if (!scene.closest_object(camera.position(), closest_obj)) {
        return;
    }
    float ray_len = closest_obj.distance;
    
    // Return a black pixel if the ray doesn't hit an object.
    uint32_t pixel_argb = 0xff000000;
    
    // March the ray until it hits an object in the scene or misses all objects.
    for (int step = 0; step != kMaxSteps && ray_len <= kMaxDist; ++step) {
        current_position = camera.position() + ray_dir * ray_len;
        
        if (!scene.closest_object(current_position, closest_obj)) {
            return;
        }
        
        if (closest_obj.distance < kEpsilon) {
            // Ray hit an object.
            
            // Determine if the point should be visible by converting it back into camera coordinates
            // and checking it against the near/far clipping planes.
            CoordTransform world_to_cam = camera.world_to_camera();
            FLOAT3 cur_pos_cam = world_to_cam.rotation * current_position + world_to_cam.translation;
            
            // Negate the z coordinate to get the point's distance from the camera.
            float dist_z = -cur_pos_cam.z;
            if (dist_z >= camera.clip_near() && dist_z <= camera.clip_far()) {
                // The position should be visible. Return a color of red.
                pixel_argb = 0xffff0000;
            }
            
            break;
        }
        
        ray_len += closest_obj.distance;
    }
    
    pixels[gid.y * output_sz.width + gid.x] = pixel_argb;
}
