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

#ifndef RECASTSAMPLE_H
#define RECASTSAMPLE_H

#include "Recast.h"
#include "SampleInterfaces.h"


/// Tool types.
enum SampleToolType
{
    TOOL_NONE = 0,
    TOOL_TILE_EDIT,
    TOOL_TILE_HIGHLIGHT,
    TOOL_TEMP_OBSTACLE,
    TOOL_NAVMESH_TESTER,
    TOOL_NAVMESH_PRUNE,
    TOOL_OFFMESH_CONNECTION,
    TOOL_CONVEX_VOLUME,
    TOOL_CROWD,
    MAX_TOOLS
};

/// These are just sample areas to use consistent values across the samples.
/// The use should specify these base on his needs.
enum SamplePolyAreas
{
    SAMPLE_POLYAREA_GROUND,
    SAMPLE_POLYAREA_WATER,
    SAMPLE_POLYAREA_ROAD,
    SAMPLE_POLYAREA_DOOR,
    SAMPLE_POLYAREA_GRASS,
    SAMPLE_POLYAREA_JUMP,
};
enum SamplePolyFlags
{
    SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
    SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
    SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
    SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
    SAMPLE_POLYFLAGS_DISABLED = 0x10,		// Disabled polygon
    SAMPLE_POLYFLAGS_ALL = 0xffff	// All abilities.
};

class SampleDebugDraw : public DebugDrawGL
{
public:
    virtual unsigned int areaToCol(unsigned int area);
};

enum SamplePartitionType
{
    SAMPLE_PARTITION_WATERSHED,
    SAMPLE_PARTITION_MONOTONE,
    SAMPLE_PARTITION_LAYERS,
};

struct SampleTool
{
    virtual ~SampleTool() {}
    virtual int type() = 0;
    virtual void init(class Sample* sample) = 0;
    virtual void reset() = 0;
    virtual void handleMenu() = 0;
    virtual void handleClick(const float* s, const float* p, bool shift) = 0;
    virtual void handleRender() = 0;
    virtual void handleRenderOverlay(double* proj, double* model, int* view) = 0;
    virtual void handleToggle() = 0;
    virtual void handleStep() = 0;
    virtual void handleUpdate(const float dt) = 0;
};

struct SampleToolState {
    virtual ~SampleToolState() {}
    virtual void init(class Sample* sample) = 0;
    virtual void reset() = 0;
    virtual void handleRender() = 0;
    virtual void handleRenderOverlay(double* proj, double* model, int* view) = 0;
    virtual void handleUpdate(const float dt) = 0;
};


#endif // RECASTSAMPLE_H
