#include "EquilateralTiling.hpp"

using namespace std;

EquilateralTiling::EquilateralTiling(int width, int height, int triangleSize) {
    hBound = width;
    vBound = height;
    triangleSideLength = triangleSize;
    createTiling(0, 0, "all", "normal");
}

void EquilateralTiling::createTiling(float xPos, float yPos, string key, string orientation) {
    if (xPos < hBound && yPos < vBound) {
        Point p1(xPos, yPos);
        Point p2(xPos + triangleSideLength, yPos);
        Point p3;
        Point p4;
        
        if (orientation=="normal") {
            p3.setX(xPos + triangleSideLength + (triangleSideLength/2));
            p3.setY(yPos + sqrt(3)/2*triangleSideLength);
            p4.setX(xPos + (triangleSideLength/2));
            p4.setY(yPos + sqrt(3)/2*triangleSideLength);
        } else {
            p3.setX(xPos + (triangleSideLength/2));
            p3.setY(yPos + sqrt(3)/2*triangleSideLength);
            p4.setX(xPos - (triangleSideLength/2));
            p4.setY(yPos + sqrt(3)/2*triangleSideLength);
        }
        
        int p1Index = check_addVertex_getIndex(p1);
        int p2Index = check_addVertex_getIndex(p2);
        int p3Index = check_addVertex_getIndex(p3);
        int p4Index = check_addVertex_getIndex(p4);
        
        Triangle t1;
        Triangle t2;
        
        if (orientation=="normal") {
            t1.setPointsAndIndices(p1, p2, p4, p1Index, p2Index, p4Index);
            t2.setPointsAndIndices(p2, p3, p4, p2Index, p3Index, p4Index);
        } else {
            t1.setPointsAndIndices(p1, p2, p3, p1Index, p2Index, p3Index);
            t2.setPointsAndIndices(p1, p3, p4, p1Index, p3Index, p4Index);
        }
        
        triangles.push_back(t1);
        triangles.push_back(t2);
        
        string oppositeKey = "";
        
        if (orientation=="normal") {
            oppositeKey = "inverted";
        } else {
            oppositeKey = "normal";
        }
        
        if (key=="up") {
            createTiling(p4.getX(), p4.getY(), "up", oppositeKey);
        } else if (key=="right") {
            createTiling(p2.getX(), p2.getY(), "right", orientation);
        } else {
            createTiling(p2.getX(), p2.getY(), "right", orientation);
            createTiling(p3.getX(), p3.getY(), "all", oppositeKey);
            createTiling(p4.getX(), p4.getY(), "up", oppositeKey);
        }
    }
}
