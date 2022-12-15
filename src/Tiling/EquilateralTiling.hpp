#ifndef EquilateralTiling_hpp
#define EquilateralTiling_hpp

#include <stdio.h>
#include "Tiling.hpp"
#include <string>
#include <cmath>
#include <iostream>

class EquilateralTiling : public Tiling {
public:
    EquilateralTiling(int, int, int);
    void createTiling(float, float, string, string) override;
};

#endif /* EquilateralTiling_hpp */
