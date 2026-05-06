//
//  object_cpu.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "object.h"

void init(Object& obj, const Sphere& sphere) noexcept;
void init(Object& obj, const Cube& cube) noexcept;
void init(Object& obj, const Box2D& box2d) noexcept;
