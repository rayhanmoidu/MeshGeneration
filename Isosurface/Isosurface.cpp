//
//  CustomIsosurface.cpp
//  isosurface_stuffing
//
//  Created by Rayhan Moidu on 2022-12-07.
//

#include "Isosurface.hpp"

Isosurface::Isosurface(float (*func)(float, float), float oX, float oY) {
    customSignedDistanceFunction = func;
    originX = oX;
    originY = oY;
}

float Isosurface::signedDistanceFunction(Point p) {
    return customSignedDistanceFunction(p.getX() + originX, p.getY() + originY);
}

int Isosurface::evaluatePoint(Point p) {
    if (signedDistanceFunction(p)==0) return 0;
    if (signedDistanceFunction(p)>0) return -1;
    return 1;
}
