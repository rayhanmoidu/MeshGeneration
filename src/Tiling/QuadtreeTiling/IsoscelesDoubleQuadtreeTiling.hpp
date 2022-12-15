//
//  IsoscelesDoubleQuadtreeTiling.hpp
//  isosurface_stuffing
//
//  Created by Rayhan Moidu on 2022-11-15.
//

#ifndef IsoscelesDoubleQuadtreeTiling_hpp
#define IsoscelesDoubleQuadtreeTiling_hpp

#include <stdio.h>
#include "QuadtreeTiling.hpp"

class IsoscelesDoubleQuadtreeTiling : public QuadtreeTiling {
public:
    IsoscelesDoubleQuadtreeTiling(SquareQuadtree);
    void createTrianglesFromCell(QuadtreeNode*) override;
};

#endif /* IsoscelesDoubleQuadtreeTiling_hpp */
