//
//  ParallelogramQuadtree.cpp
//  isosurface_stuffing
//
//  Created by Rayhan Moidu on 2022-11-15.
//

#include "ParallelogramQuadtree.hpp"
#include "Quadtree.hpp"
#include <cmath>
#include <iostream>

ParallelogramQuadtree::ParallelogramQuadtree(int screenWidth, int screenHeight, int smallestGridSize, Isosurface &isosurface) : Quadtree(isosurface) {
    hBound = screenWidth;
    vBound = screenHeight;
    gridSizeLimit = smallestGridSize;
    refineAroundIsosurface = true;
    originX = 0;
    originY = 0;
    
    float newH = 4*hBound/3;
    
    float curDimension = 2*newH/sqrt(3);
    float curCenterX = hBound/2;
    float curCenterY = hBound/2;

    root = new QuadtreeNode(curCenterX, curCenterY, curDimension, NULL);

    if (shouldRefine(root)) {
        constructChildren(root);
    }
    balanceQuadtree();
}

ParallelogramQuadtree::ParallelogramQuadtree(int screenWidth, int screenHeight, float cellToTraingleSizeRatio, float (*func)(float, float), int probingValue, float oX, float oY) : Quadtree() {
    hBound = screenWidth;
    vBound = screenHeight;
    gridSizeLimit = 0;
    refineAroundIsosurface = false;
    tilingCellSizeToMaxTriangleSizeRatio = cellToTraingleSizeRatio;
    sizingFunction = func;
    probingDistance = probingValue;
    originX = oX;
    originY = oY;

    root = new QuadtreeNode(hBound / 2, vBound / 2, hBound, NULL);

    if (shouldRefine(root)) {
        constructChildren(root);
    }
    balanceQuadtree();
}

void ParallelogramQuadtree::constructChildren(QuadtreeNode *node) {
    
    float shiftX = (node->getDimension() / 2) * (tan60) / (tan30+tan60);
    float shiftY = shiftX*tan30;
    
    float newCurX1 = node->getCenterX() - node->getDimension() / 4;
    float newCurY1 = node->getCenterY() - getCellHeight(node)/2;
    
    float newCurX2 = node->getCenterX() + node->getDimension() / 4;
    float newCurY2 = node->getCenterY() + getCellHeight(node)/2;
    
    QuadtreeNode* northEastChild = new QuadtreeNode(node->getCenterX() + shiftX, node->getCenterY() + shiftY, node->getDimension() / 2, node);
    QuadtreeNode* northWestChild = new QuadtreeNode(newCurX2 - shiftX, newCurY2 - shiftY, node->getDimension() / 2, node);
    QuadtreeNode* southEastChild = new QuadtreeNode(newCurX1 + shiftX, newCurY1 + shiftY, node->getDimension() / 2, node);
    QuadtreeNode* southWestChild = new QuadtreeNode(node->getCenterX() - shiftX, node->getCenterY() - shiftY, node->getDimension() / 2, node);

    if (shouldRefine(northEastChild)) constructChildren(northEastChild);
    if (shouldRefine(northWestChild)) constructChildren(northWestChild);
    if (shouldRefine(southEastChild)) constructChildren(southEastChild);
    if (shouldRefine(southWestChild)) constructChildren(southWestChild);

    node->addChild(northEastChild, northeast);
    node->addChild(northWestChild, northwest);
    node->addChild(southEastChild, southeast);
    node->addChild(southWestChild, southwest);
}

bool ParallelogramQuadtree::shouldRefine(QuadtreeNode* node) {
    if (refineAroundIsosurface) {
        float diagonalLength = (2*(getCellHeight(node)));

        Point corner1(node->getCenterX() + (node->getDimension()/2), node->getCenterY() + (node->getDimension()/2));
        Point corner2(node->getCenterX() - (node->getDimension()/2), node->getCenterY() + (node->getDimension()/2));
        Point corner3(node->getCenterX() + (node->getDimension()/2), node->getCenterY() - (node->getDimension()/2));
        Point corner4(node->getCenterX() - (node->getDimension()/2), node->getCenterY() - (node->getDimension()/2));

        if (node->getDimension()/2 < gridSizeLimit) return false;
        if (abs(isosurface.signedDistanceFunction(corner1)) > diagonalLength) return false;
        if (abs(isosurface.signedDistanceFunction(corner4)) > diagonalLength) return false;
        if (abs(isosurface.signedDistanceFunction(corner2)) > diagonalLength) return false;
        if (abs(isosurface.signedDistanceFunction(corner3)) > diagonalLength) return false;
        return true;
    } else {
        Point curProbePoint(node->getCenterX() - (node->getDimension() / 2), node->getCenterY() + (node->getDimension() / 2));
        
        bool shouldRefine = false;
        while (curProbePoint.getY() >= node->getCenterY() - (node->getDimension() / 2)) {
            // TEST PROBE POINT
            float maxTriangleSideLength = sizingFunction(curProbePoint.getX() + originX, curProbePoint.getY() + originY);
            
            if (maxTriangleSideLength < node->getDimension()/tilingCellSizeToMaxTriangleSizeRatio) {
                shouldRefine = true;
                break;
            }
            
            // ADJUST PROBE POINT
            float newX = curProbePoint.getX() + probingDistance;
            float newY = curProbePoint.getY();
            if (newX > node->getCenterX() + (node->getDimension() / 2)) {
                newX = node->getCenterX() - (node->getDimension() / 2);
                newY = curProbePoint.getY() - probingDistance;
            }
            Point newProbePoint(newX, newY);
            curProbePoint = newProbePoint;
        }
        return shouldRefine;
    }
}

float ParallelogramQuadtree::getCellHeight(QuadtreeNode* curNode) {
    return sqrt(3)*curNode->getDimension()/2;
}

void ParallelogramQuadtree::refineNode(QuadtreeNode* node) {
    float shiftX = (node->getDimension() / 2) * (tan60) / (tan30+tan60);
    float shiftY = shiftX*tan30;
    
    float newCurX1 = node->getCenterX() - node->getDimension() / 4;
    float newCurY1 = node->getCenterY() - getCellHeight(node)/2;
    
    float newCurX2 = node->getCenterX() + node->getDimension() / 4;
    float newCurY2 = node->getCenterY() + getCellHeight(node)/2;
    
    QuadtreeNode* northEastChild = new QuadtreeNode(node->getCenterX() + shiftX, node->getCenterY() + shiftY, node->getDimension() / 2, node);
    QuadtreeNode* northWestChild = new QuadtreeNode(newCurX2 - shiftX, newCurY2 - shiftY, node->getDimension() / 2, node);
    QuadtreeNode* southEastChild = new QuadtreeNode(newCurX1 + shiftX, newCurY1 + shiftY, node->getDimension() / 2, node);
    QuadtreeNode* southWestChild = new QuadtreeNode(node->getCenterX() - shiftX, node->getCenterY() - shiftY, node->getDimension() / 2, node);

    node->addChild(northEastChild, northeast);
    node->addChild(northWestChild, northwest);
    node->addChild(southEastChild, southeast);
    node->addChild(southWestChild, southwest);
}


