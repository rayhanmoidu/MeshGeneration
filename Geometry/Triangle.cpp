#include "Triangle.hpp"
#include "Point.hpp"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cmath>

Triangle::Triangle(Point triangleP1, Point triangleP2, Point triangleP3, int index1, int index2, int index3) {
    p1 = triangleP1;
    p2 = triangleP2;
    p3 = triangleP3;
    i1 = index1;
    i2 = index2;
    i3 = index3;
    cutpoints = vector<Cutpoint>();
    setColors();
}

Triangle::Triangle() {
    p1 = Point();
    p2 = Point();
    p3 = Point();
    i1 = 0;
    i2 = 0;
    i3 = 0;
    cutpoints = vector<Cutpoint>();
    setColors();
}

void Triangle::setPointsAndIndices(Point point1, Point point2, Point point3, int index1, int index2, int index3) {
    p1 = point1;
    p2 = point2;
    p3 = point3;
    i1 = index1;
    i2 = index2;
    i3 = index3;
}

vector<Point> Triangle::getPoints() {
    vector<Point> points = vector<Point>();
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    return points;
}

vector<int> Triangle::getIndices() {
    vector<int> indices = vector<int>();
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
    return indices;
}

void Triangle::addCutpoint(Cutpoint newCutpoint) {
    cutpoints.push_back(newCutpoint);
}

vector<Cutpoint> Triangle::getCutpoints() {
    return cutpoints;
}

bool Triangle::doesContainVertex(Point p) {
    return p1==p || p2==p || p3==p;
}

bool Triangle::doesContainIndex(int i) {
    return i1==i || i2==i || i3==i;
}

bool Triangle::doesContainCutpoint(Cutpoint cp) {
    for (int i = 0; i < cutpoints.size(); i++) {
        if (cutpoints[i]==cp) return true;
    }
    return false;
}

void Triangle::warpVertexToCutpoint(Point p, Cutpoint cp, int newIndex) {
    if (p1==p) {
        p1 = Point(cp.getX(), cp.getY());
        i1 = newIndex;
    }
    if (p2==p) {
        p2 = Point(cp.getX(), cp.getY());
        i2 = newIndex;
    }
    if (p3==p) {
        p3 = Point(cp.getX(), cp.getY());
        i3 = newIndex;
    }
}

void Triangle::removeCutpoint(Cutpoint cp) {
    vector<Cutpoint> newCutpoints;
    for (int i = 0; i < cutpoints.size(); i++) {
        if (!(cutpoints[i]==cp)) {
            newCutpoints.push_back(cutpoints[i]);
        }
    }
    cutpoints = newCutpoints;
}

void Triangle::removeAllCutpoints() {
    cutpoints = vector<Cutpoint>();
}

bool Triangle::operator == (Triangle &obj) {
    bool doCutpointsMatch = true;
    for (int i = 0; i < cutpoints.size(); i++) {
        bool foundCurCutpoint = false;
        for (int j = 0; j < obj.cutpoints.size(); j++) {
            if (cutpoints[i]==obj.cutpoints[j]) {
                foundCurCutpoint = true;
            }
        }
        if (!foundCurCutpoint) doCutpointsMatch = false;
    }
    
    bool pointsMatch1 = (p1==obj.p1) || (p1==obj.p2) || (p1==obj.p3);
    bool pointsMatch2 = (p2==obj.p1) || (p2==obj.p2) || (p2==obj.p3);
    bool pointsMatch3 = (p3==obj.p1) || (p3==obj.p2) || (p3==obj.p3);

    
    return (pointsMatch1 && pointsMatch2 && pointsMatch3) && doCutpointsMatch;
}

float Triangle::applyCosineLaw(float a, float b, float c) {
    return (acos((b*b + c*c - a*a) / (2*b*c))) * (180/M_PI);
}

vector<float> Triangle::computeAngles() {
    vector<float> angles;
    
    float dx1 = p1.getX() - p2.getX();
    float dy1 = p1.getY() - p2.getY();
    
    float dx2 = p2.getX() - p3.getX();
    float dy2 = p2.getY() - p3.getY();
    
    float dx3 = p3.getX() - p1.getX();
    float dy3 = p3.getY() - p1.getY();
    
    float a = sqrt(dx1*dx1 + dy1*dy1);
    float b = sqrt(dx2*dx2 + dy2*dy2);
    float c = sqrt(dx3*dx3 + dy3*dy3);
    
    float angle1 = applyCosineLaw(a, b, c);
    float angle2 = applyCosineLaw(b, c, a);
    float angle3 = applyCosineLaw(c, a, b);
    
    angles.push_back(angle1);
    angles.push_back(angle2);
    angles.push_back(angle3);
    
    return angles;
}
