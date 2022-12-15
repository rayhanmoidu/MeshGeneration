#include "meshGeneration.hpp"

int getCellToTriangleSizeRatio(TilingType tilingType) {
    if (tilingType==EQUILATERAL || tilingType==ISOSCELES) cerr << "ERROR: Attempting to compute quadtree cell size limit for uniform mesh." << endl;
    if (tilingType==EQUILATERAL_ADAPTIVE) return 1; // minimum side length is minimum cell dimension
    if (tilingType==ISOSCELES_SINGLE_ADAPTIVE) return 1/sqrt(2);
    if (tilingType==ISOSCELES_DOUBLE_ADAPTIVE) return sqrt(2);
    if (tilingType==ALL_ACUTE_ADAPTIVE) return 100/52.7046; // figured this out through trials
    return 0;
}

pair<vector<pair<float, float>>, vector<vector<int>>> generateMesh(float (*signedDistanceFunction)(float, float), float isosurfaceWidth, float isosurfaceHeight, float originX, float originY, TilingType tilingType, float minimumTriangleSideLength) {
    float maxDimension = max(isosurfaceWidth, isosurfaceHeight);
    int minimumCellSize = getCellToTriangleSizeRatio(tilingType) * minimumTriangleSideLength;
    Isosurface isosurface(signedDistanceFunction, originX, originY);
    Tiling *triangleTiling;

    switch(tilingType) {
        case EQUILATERAL:
            triangleTiling = new EquilateralTiling(isosurfaceWidth, isosurfaceHeight, minimumTriangleSideLength);
            break;
        case ISOSCELES:
            triangleTiling = new IsoscelesTiling(isosurfaceWidth, isosurfaceHeight, minimumTriangleSideLength);
            break;
        case ISOSCELES_SINGLE_ADAPTIVE:
            SquareQuadtree quadtree1 = SquareQuadtree(maxDimension, maxDimension, minimumCellSize, isosurface);
            triangleTiling = new IsoscelesSingleQuadtreeTiling(quadtree1);
            break;
        case ISOSCELES_DOUBLE_ADAPTIVE:
            SquareQuadtree quadtree2 = SquareQuadtree(maxDimension, maxDimension, minimumCellSize, isosurface);
            triangleTiling = new IsoscelesDoubleQuadtreeTiling(quadtree2);
            break;
        case EQUILATERAL_ADAPTIVE:
            ParallelogramQuadtree quadtree3 = ParallelogramQuadtree(maxDimension, maxDimension, minimumCellSize, isosurface);
            triangleTiling = new EquilateralQuadtreeTiling(quadtree3);
            break;
        case ALL_ACUTE_ADAPTIVE:
            SquareQuadtree quadtree4 = SquareQuadtree(maxDimension, maxDimension, minimumCellSize, isosurface);
            triangleTiling = new ProvablyGoodQuadtreeTiling(quadtree4);
            break;
    }

    Algorithm algorithmInstance(triangleTiling, isosurface, alpha);
    algorithmInstance.execute();

    return algorithmInstance.getOutput(originX, originY);
}

pair<vector<pair<float, float>>, vector<vector<int>>> generateMesh(float (*signedDistanceFunction)(float, float), float isosurfaceWidth, float isosurfaceHeight, float originX, float originY, TilingType tilingType, float (*sizingFunction)(float, float), float maxSizingRateOfChange) {
    if (tilingType == EQUILATERAL || tilingType == ISOSCELES) cerr << "ERROR: Provided tiling type is not adaptive." << endl;
    float maxDimension = max(isosurfaceWidth, isosurfaceHeight);

    Isosurface isosurface(signedDistanceFunction, originX, originY);
    Tiling *triangleTiling;

    switch(tilingType) {
        case ISOSCELES_SINGLE_ADAPTIVE:
            SquareQuadtree quadtree1 = SquareQuadtree(maxDimension, maxDimension, getCellToTriangleSizeRatio(tilingType), sizingFunction, maxSizingRateOfChange, originX, originY);
            triangleTiling = new IsoscelesSingleQuadtreeTiling(quadtree1);
            break;
        case ISOSCELES_DOUBLE_ADAPTIVE:
            SquareQuadtree quadtree2 = SquareQuadtree(maxDimension, maxDimension, getCellToTriangleSizeRatio(tilingType), sizingFunction, maxSizingRateOfChange, originX, originY);
            triangleTiling = new IsoscelesDoubleQuadtreeTiling(quadtree2);
            break;
        case EQUILATERAL_ADAPTIVE:
            ParallelogramQuadtree quadtree3 = ParallelogramQuadtree(maxDimension, maxDimension, getCellToTriangleSizeRatio(tilingType), sizingFunction, maxSizingRateOfChange, originX, originY);
            triangleTiling = new EquilateralQuadtreeTiling(quadtree3);
            break;
        case ALL_ACUTE_ADAPTIVE:
            SquareQuadtree quadtree4 = SquareQuadtree(maxDimension, maxDimension, getCellToTriangleSizeRatio(tilingType), sizingFunction, maxSizingRateOfChange, originX, originY);
            triangleTiling = new ProvablyGoodQuadtreeTiling(quadtree4);
            break;
    }

    Algorithm algorithmInstance(triangleTiling, isosurface, alpha);
    algorithmInstance.execute();

    return algorithmInstance.getOutput(originX, originY);
}

// note to self: 
// - correct usage of alpha
// create library
// test out the code




//FINAL TODO

// FIGURE OUT THE INT RETURN VAL OF RATIOS -> MAKE SURE RATIOS R GOOD
// FIGURE OUT ALPHA