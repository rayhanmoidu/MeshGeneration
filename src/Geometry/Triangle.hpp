#ifndef Triangle_hpp
#define Triangle_hpp

#include <stdio.h>
#include "Point.hpp"
#include "Cutpoint.hpp"
#include <vector>

using namespace std;

class Triangle {
public:
    Triangle(Point, Point, Point, int, int, int);
    Triangle();
    bool doesContainCutpoint(Cutpoint);
    bool doesContainVertex(Point);
    bool doesContainIndex(int);
    void warpVertexToCutpoint(Point, Cutpoint, int);
    void removeCutpoint(Cutpoint);
    void removeAllCutpoints();
    void addCutpoint(Cutpoint);
    vector<float> computeAngles();
    
    void setPointsAndIndices(Point, Point, Point, int, int, int);
    vector<Point> getPoints();
    vector<Cutpoint> getCutpoints();
    vector<int> getIndices();
    
    bool operator == (Triangle &obj);
    bool doIndicesMatch(Triangle obj);
    
private:
    float applyCosineLaw(float, float, float);
    
    Point p1;
    Point p2;
    Point p3;
    vector<Cutpoint> cutpoints;
    
    int i1;
    int i2;
    int i3;
};

#endif /* Triangle_hpp */
