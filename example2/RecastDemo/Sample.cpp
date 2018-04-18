//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include "Sample.h"

unsigned int SampleDebugDraw::areaToCol(unsigned int area)
{
    switch (area)
    {
        // Ground (0) : light blue
    case SAMPLE_POLYAREA_GROUND: return duRGBA(0, 192, 255, 255);
        // Water : blue
    case SAMPLE_POLYAREA_WATER: return duRGBA(0, 0, 255, 255);
        // Road : brown
    case SAMPLE_POLYAREA_ROAD: return duRGBA(50, 20, 12, 255);
        // Door : cyan
    case SAMPLE_POLYAREA_DOOR: return duRGBA(0, 255, 255, 255);
        // Grass : green
    case SAMPLE_POLYAREA_GRASS: return duRGBA(0, 255, 0, 255);
        // Jump : yellow
    case SAMPLE_POLYAREA_JUMP: return duRGBA(255, 255, 0, 255);
        // Unexpected : red
    default: return duRGBA(255, 0, 0, 255);
    }
}
