//
//  object.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "shared.h"
#include "sphere.h"
#include "cube.h"
#include "box2d.h"

enum class ObjectType {
    Unused,
    Sphere,
    Cube,
    Box2D
};

struct Object {
    ObjectType type = ObjectType::Unused;
    union {
        uint8_t unused = 0;
        Sphere sphere;
        Cube cube;
        Box2D box2d;
    };
};
