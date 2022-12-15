#ifndef Isosurface_hpp
#define Isosurface_hpp

#include <stdio.h>
#include "Point.hpp"

class Isosurface {
public:
    Isosurface(float (*)(float, float), float, float);
    float signedDistanceFunction(Point);
    int evaluatePoint(Point);
    
private:
    float (*customSignedDistanceFunction)(float, float);
    float originX;
    float originY;
};

#endif /* Isosurface_hpp */
