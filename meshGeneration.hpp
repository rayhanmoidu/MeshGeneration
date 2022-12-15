#ifndef MeshGeneration_hpp
#define MeshGeneration_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Triangle.hpp"
#include "Isosurface.hpp"
#include "Tiling.hpp"
#include "EquilateralTiling.hpp"
#include "IsoscelesTiling.hpp"
#include "IsoscelesSingleQuadtreeTiling.hpp"
#include "IsoscelesDoubleQuadtreeTiling.hpp"
#include "EquilateralQuadtreeTiling.hpp"
#include "ProvablyGoodQuadtreeTiling.hpp"
#include "Algorithm.hpp"

enum TilingType {ISOSCELES, EQUILATERAL, EQUILATERAL_ADAPTIVE, ISOSCELES_SINGLE_ADAPTIVE, ISOSCELES_DOUBLE_ADAPTIVE, ALL_ACUTE_ADAPTIVE };
int getCellToTriangleSizeRatio(TilingType tilingType);

// might need to figure out how to deal with alpha
const float alpha = 60;

// Refines around surface
pair<vector<pair<float, float>>, vector<vector<int>>> generateMesh(float (*signedDistanceFunction)(float, float), float isosurfaceWidth, float isosurfaceHeight, float originX, float originY, TilingType tilingType, float minimumTriangleSideLength);
// Manual refinement
pair<vector<pair<float, float>>, vector<vector<int>>> generateMesh(float (*signedDistanceFunction)(float, float), float isosurfaceWidth, float isosurfaceHeight, float originX, float originY, TilingType tilingType, float (*sizingFunction)(float, float), float maxSizingROC);

#endif /* MeshGeneration_hpp */