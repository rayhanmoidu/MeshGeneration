//
//  SquareQuadtree.cpp
//  isosurface_stuffing
//
//  Created by Rayhan Moidu on 2022-11-15.
//

#include "SquareQuadtree.hpp"
#include "Quadtree.hpp"
#include <cmath>
#include <iostream>

SquareQuadtree::SquareQuadtree(int screenWidth, int screenHeight, int smallestGridSize, Isosurface &isosurface) : Quadtree(isosurface) {
    hBound = screenWidth;
    vBound = screenHeight;
    gridSizeLimit = smallestGridSize;
    refineAroundIsosurface = true;
    originX = 0;
    originY = 0;

    root = new QuadtreeNode(hBound / 2, vBound / 2, hBound, NULL);

    if (shouldRefine(root)) {
        constructChildren(root);
    }
    balanceQuadtree();
    
    if (isBalanced()) {
        cout <<"correctly balanced!"<<endl;
    } else {
        cout <<"ERROR incorrectly balanced!"<<endl;
    }
}

SquareQuadtree::SquareQuadtree(int screenWidth, int screenHeight, int cellToTraingleSizeRatio, float (*func)(float, float), int probingValue, float oX, float oY) : Quadtree() {
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
    
    if (isBalanced()) {
        cout <<"correctly balanced!"<<endl;
    } else {
        cout <<"ERROR incorrectly balanced!"<<endl;
    }
}


void SquareQuadtree::constructChildren(QuadtreeNode *node) {
    QuadtreeNode* northEastChild = new QuadtreeNode(node->getCenterX() + node->getDimension()/4, node->getCenterY() + node->getDimension()/4, node->getDimension() / 2, node);
    QuadtreeNode* northWestChild = new QuadtreeNode(node->getCenterX() - node->getDimension()/4, node->getCenterY() + node->getDimension()/4, node->getDimension() / 2, node);
    QuadtreeNode* southEastChild = new QuadtreeNode(node->getCenterX() + node->getDimension()/4, node->getCenterY() - node->getDimension()/4, node->getDimension() / 2, node);
    QuadtreeNode* southWestChild = new QuadtreeNode(node->getCenterX() - node->getDimension()/4, node->getCenterY() - node->getDimension()/4, node->getDimension() / 2, node);

    if (shouldRefine(northEastChild)) constructChildren(northEastChild);
    if (shouldRefine(northWestChild)) constructChildren(northWestChild);
    if (shouldRefine(southEastChild)) constructChildren(southEastChild);
    if (shouldRefine(southWestChild)) constructChildren(southWestChild);

    node->addChild(northEastChild, northeast);
    node->addChild(northWestChild, northwest);
    node->addChild(southEastChild, southeast);
    node->addChild(southWestChild, southwest);
}

bool SquareQuadtree::shouldRefine(QuadtreeNode* node) {
    if (refineAroundIsosurface) {
        float diagonalLength = (sqrt(node->getDimension()*node->getDimension()*2));

        Point corner1(node->getCenterX() + (node->getDimension()/2), node->getCenterY() + (node->getDimension()/2));
        Point corner2(node->getCenterX() - (node->getDimension()/2), node->getCenterY() + (node->getDimension()/2));
        Point corner3(node->getCenterX() + (node->getDimension()/2), node->getCenterY() - (node->getDimension()/2));
        Point corner4(node->getCenterX() - (node->getDimension()/2), node->getCenterY() - (node->getDimension()/2));

        if (node->getDimension() < gridSizeLimit) return false;
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

void SquareQuadtree::refineNode(QuadtreeNode* node) {
    QuadtreeNode* northEastChild = new QuadtreeNode(node->getCenterX() + node->getDimension()/4, node->getCenterY() + node->getDimension()/4, node->getDimension() / 2, node);
    QuadtreeNode* northWestChild = new QuadtreeNode(node->getCenterX() - node->getDimension()/4, node->getCenterY() + node->getDimension()/4, node->getDimension() / 2, node);
    QuadtreeNode* southEastChild = new QuadtreeNode(node->getCenterX() + node->getDimension()/4, node->getCenterY() - node->getDimension()/4, node->getDimension() / 2, node);
    QuadtreeNode* southWestChild = new QuadtreeNode(node->getCenterX() - node->getDimension()/4, node->getCenterY() - node->getDimension()/4, node->getDimension() / 2, node);

    node->addChild(northEastChild, northeast);
    node->addChild(northWestChild, northwest);
    node->addChild(southEastChild, southeast);
    node->addChild(southWestChild, southwest);
}
