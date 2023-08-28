// Copyright Antioch. All rights reserved.

#pragma once

#include <cstddef>

namespace antioch::gfx {

struct DeviceMemory_t;
struct Buffer_t;
struct Image_t;
struct Brush_t;
struct Glyph_t;

typedef void* (*PFN_allocateFunction)(void* pUserData, size_t size);
typedef void (*PFN_freeFunction)(void* pUserData, void* memory);

typedef DeviceMemory_t* DeviceMemory;
typedef Buffer_t* Buffer;
typedef Image_t* Image;
typedef Brush_t* Brush;
typedef Glyph_t* Glyph;
}  // namespace antioch::gfx
