//
//  QuadtreeTiling.hpp
//  isosurface_stuffing
//
//  Created by Rayhan Moidu on 2022-11-02.
//

#ifndef QuadtreeTiling_hpp
#define QuadtreeTiling_hpp

#include <stdio.h>
#include "../../Quadtree/Quadtree.hpp"
#include "../../Tiling/Tiling.hpp"
#include "../../Geometry/Point.hpp"
#include "../../Quadtree/SquareQuadtree.hpp"
#include "../../Quadtree/ParallelogramQuadtree.hpp"
#include <unordered_set>
#include <utility>
#include <iostream>
#include <cmath>
#include <vector>

class QuadtreeTiling : public Tiling {
public:
    QuadtreeTiling();
    void createTiling(float, float, string, string) override;
    
protected:
    void createTilingHelper(QuadtreeNode*);
    
    virtual void createTrianglesFromCell(QuadtreeNode*) = 0;
};

#endif /* QuadtreeTiling_hpp */
