cmake_minimum_required(VERSION 3.5)

project(MeshGeneration)

############################################################
# Create a library
############################################################

#Generate the shared library from the library sources
add_library(MeshGeneration SHARED 
    src/meshGeneration.cpp
    src/Isosurface/Isosurface.cpp
    src/Algorithm/Algorithm.cpp
    src/Quadtree/Quadtree.cpp
    src/Quadtree/QuadtreeNode.cpp
    src/Quadtree/SquareQuadtree.cpp
    src/Quadtree/ParallelogramQuadtree.cpp
    src/Geometry/Point.cpp
    src/Geometry/Cutpoint.cpp
    src/Geometry/Triangle.cpp
    src/Tiling/Tiling.cpp
    src/Tiling/IsoscelesTiling.cpp
    src/Tiling/EquilateralTiling.cpp
    src/Tiling/QuadtreeTiling/QuadtreeTiling.cpp
    src/Tiling/QuadtreeTiling/EquilateralQuadtreeTiling.cpp
    src/Tiling/QuadtreeTiling/IsoscelesSingleQuadtreeTiling.cpp
    src/Tiling/QuadtreeTiling/IsoscelesDoubleQuadtreeTiling.cpp
    src/Tiling/QuadtreeTiling/ProvablyGood/ProvablyGoodQuadtreeTiling.cpp
    src/Tiling/QuadtreeTiling/ProvablyGood/ProvablyGoodTriangleFactory.cpp
)

target_include_directories(MeshGeneration
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

set(public_headers
    include/meshGeneration.hpp
)
# note that ${public_headers} has to be in quotes
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")

############################################################
# Create an executable
############################################################

# # Add an executable with the above sources
# add_executable(MeshGeneration_binary
#     src/main.cpp
# )

# # link the new hello_library target with the hello_binary target
# target_link_libraries( hello_binary
#     PRIVATE 
#         hello::library
# )