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

#include <SDL3/SDL.h>

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
    // ----------------------------------------------------------------------
    // Initialize SDL and create the window.
    // ----------------------------------------------------------------------
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "ERROR: SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "GPU Raymarcher",
        kOutputSize.width, kOutputSize.height,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::cerr << "ERROR: SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        return 1;
    }

    // ----------------------------------------------------------------------
    // Create renderer and streaming texture.
    // A streaming texture is the fastest CPU blit (block transfer) path.
    // ----------------------------------------------------------------------
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "ERROR: SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        kOutputSize.width, kOutputSize.height
    );
    if (!texture) {
        std::cerr << "ERROR: SDL_CreateTexture failed: " << SDL_GetError() << "\n";
        return 1;
    }
    
    // Metal creates a MTLDevice for each GPU.
    // If the system has multiple GPUs, Metal chooses one as the default.
    // Get the default GPU.
    auto device = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
    if (!device) {
        std::cerr << "ERROR: failed to create the system default device\n";
        return 1;
    }
    
    // Allocate the shared pixel buffer.
    // Metal manages each buffer as an opaque collection of bytes. The format is specified when it's used in the shader.
    auto pixels = NS::TransferPtr(
        device->newBuffer(kOutputSize.width * kOutputSize.height * sizeof(uint32_t),
                          MTL::ResourceStorageModeShared)
    );
    if (!pixels) {
        std::cerr << "ERROR: failed to create the shared pixel buffer\n";
        return 1;
    }
    
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
    
    Raymarcher marcher{device, pixels};
    std::string marcher_error;
    if (!marcher.init(marcher_error)) {
        std::cerr << "ERROR: failed to initialize the ray marcher: " << marcher_error << "\n";
        return 1;
    }
    
    // ----------------------------------------------------------------------
    // Main Loop.
    // ----------------------------------------------------------------------
    SDL_Event event;
    bool done = false;
    while (true) {
        // Handle events.
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }
        if (done) {
            break;
        }

        // Compute pixels on the GPU.
        

        // Update SDL texture with pixel buffer
        SDL_UpdateTexture(texture, nullptr, pixels->contents(), kOutputSize.width * sizeof(uint32_t));

        // Render the texture to the window
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    // ----------------------------------------------------------------------
    // Cleanup
    // ----------------------------------------------------------------------
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
