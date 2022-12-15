#ifndef Algorithm_hpp
#define Algorithm_hpp

#include <stdio.h>
#include "Tiling.hpp"
#include "Isosurface.hpp"

class Algorithm {
public:
    Algorithm(Tiling*, Isosurface&, float);
    void execute();
    
    // getters
    pair<vector<pair<float, float>>, vector<vector<int>>> getOutput(float, float);
    
private:
    Tiling *baseTiling;
    Isosurface &isosurface;
    float alpha;
    
    vector<Triangle> allTriangles;
    vector<Triangle> processedTriangles;
    
    // ALGORITHM STEPS
    // step 1
    void findTrianglesOverlayingIsosurface();
    // step 2
    void findCutPoints();
    // step 3
    void warpPerimeterTriangles();
    // step 4
    void clipPerimeterTriangles();
    
    // helpers
    vector<vector<int>> getTriangleIndices();
    vector<Triangle*> getPerimeterTrianglesForProcessing();
    Point interpolateCutpoint(Point, Point);
    Point findCutpointLocationBetweenPolarPoints(Point, Point);
    float distanceBetweenTwoPoints(Point, Point);
    vector<vector<Point>> sliceTrianglePointsBySign(vector<Point>);
    void filterOutNegativeTriangles();
    bool doesPassDelaunaysCondition(Point, Point, Point, Point);
};

#endif /* Algorithm_hpp */
