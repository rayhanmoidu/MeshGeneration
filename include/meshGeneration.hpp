#ifndef MeshGeneration_hpp
#define MeshGeneration_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../src/Geometry/Triangle.hpp"
#include "../src/Isosurface/Isosurface.hpp"
#include "../src/Tiling/Tiling.hpp"
#include "../src/Tiling/EquilateralTiling.hpp"
#include "../src/Tiling/IsoscelesTiling.hpp"
#include "../src/Tiling/QuadtreeTiling/IsoscelesSingleQuadtreeTiling.hpp"
#include "../src/Tiling/QuadtreeTiling/IsoscelesDoubleQuadtreeTiling.hpp"
#include "../src/Tiling/QuadtreeTiling/EquilateralQuadtreeTiling.hpp"
#include "../src/Tiling/QuadtreeTiling/ProvablyGood/ProvablyGoodQuadtreeTiling.hpp"
#include "../src/Algorithm/Algorithm.hpp"

enum TilingType {ISOSCELES, EQUILATERAL, EQUILATERAL_ADAPTIVE, ISOSCELES_SINGLE_ADAPTIVE, ISOSCELES_DOUBLE_ADAPTIVE, ALL_ACUTE_ADAPTIVE };
enum RatioType {MAX, MIN};
float getCellToTriangleSizeRatio(TilingType tilingType, RatioType ratioType);

const float alpha = 0.25;

// Refines around surface
pair<vector<pair<float, float> >, vector<vector<int> > > generateMesh(float (*signedDistanceFunction)(float, float), float isosurfaceWidth, float isosurfaceHeight, float originX, float originY, TilingType tilingType, float minimumTriangleSideLength);
// Manual refinement
pair<vector<pair<float, float> >, vector<vector<int> > > generateMesh(float (*signedDistanceFunction)(float, float), float isosurfaceWidth, float isosurfaceHeight, float originX, float originY, TilingType tilingType, float (*sizingFunction)(float, float), float maxSizingROC);

#endif /* MeshGeneration_hpp */