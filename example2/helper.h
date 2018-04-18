#pragma once

#include "RecastDemo/MeshLoaderObj.h"
#include <memory>

enum DrawMode
{
    DRAWMODE_NAVMESH,
    DRAWMODE_NAVMESH_TRANS,
    DRAWMODE_NAVMESH_BVTREE,
    DRAWMODE_NAVMESH_NODES,
    DRAWMODE_NAVMESH_INVIS,
    DRAWMODE_MESH,
    DRAWMODE_VOXELS,
    DRAWMODE_VOXELS_WALKABLE,
    DRAWMODE_COMPACT,
    DRAWMODE_COMPACT_DISTANCE,
    DRAWMODE_COMPACT_REGIONS,
    DRAWMODE_REGION_CONNECTIONS,
    DRAWMODE_RAW_CONTOURS,
    DRAWMODE_BOTH_CONTOURS,
    DRAWMODE_CONTOURS,
    DRAWMODE_POLYMESH,
    DRAWMODE_POLYMESH_DETAIL,
    MAX_DRAWMODE
};

int InitWindow(
    void(*OnUpdate)(float delta),
    void(*OnRender)(),
    void(*OnRenderOverlay)(double* proj, double* model, int* view));

class Mesh {
public:
    rcMeshLoaderObj mesh;
    float meshBoundsMin[3];
    float meshBoundsMax[3];
};


std::shared_ptr<Mesh> LoadMesh(const std::string& filepath);

void RenderMesh(const std::shared_ptr<Mesh>& mesh, float cellSize, DrawMode drawMode);