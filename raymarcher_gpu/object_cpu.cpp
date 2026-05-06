//
//  object_cpu.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 5/5/26.
//

#include "object_cpu.h"

void init(Object& obj, const Sphere& sphere) noexcept {
    obj.type = ObjectType::Sphere;
    obj.sphere = sphere;
}

void init(Object& obj, const Cube& cube) noexcept {
    obj.type = ObjectType::Cube;
    obj.cube = cube;
}

void init(Object& obj, const Box2D& box2d) noexcept {
    obj.type = ObjectType::Box2D;
    obj.box2d = box2d;
}
