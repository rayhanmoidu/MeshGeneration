//
//  Quadtree.hpp
//  isosurface_stuffing
//
//  Created by Rayhan Moidu on 2022-10-26.
//

#ifndef Quadtree_hpp
#define Quadtree_hpp

#include <stdio.h>
#include "Isosurface.hpp"
#include <vector>
#include <utility>
#include <iostream>
#include "QuadtreeNode.hpp"
//#include <pair>

using namespace std;

class Quadtree {
public:
    Quadtree(int, int, int, Isosurface&);
    Quadtree(Isosurface&);
    Quadtree();
    
    QuadtreeNode* getRoot();
    Isosurface& getIsosurface();
    
    vector<QuadtreeNode*> getListOfLeaves();
    virtual void refineNode(QuadtreeNode*) = 0;
    vector<QuadtreeNode*> removeNodeFromVector(vector<QuadtreeNode*>, QuadtreeNode*);
    
protected:
    virtual bool shouldRefine(QuadtreeNode*) = 0;
    virtual void constructChildren(QuadtreeNode*) = 0;
    
    bool doesContainLeaf(vector<QuadtreeNode*>, QuadtreeNode*);
    void balanceQuadtree();
    bool isBalanced();
    
    int gridSizeLimit;
    int hBound;
    int vBound;
    
    bool refineAroundIsosurface;
    int tilingCellSizeToMaxTriangleSizeRatio = 0;
    float (*sizingFunction)(float, float);
    int probingDistance;
    float originX;
    float originY;
        
    Isosurface &isosurface;
    QuadtreeNode* root;
};

#endif /* Quadtree_hpp */
