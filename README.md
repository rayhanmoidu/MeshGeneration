# MeshGeneration
The purpose of this library is to generate 2D triangular meshes for user-defined isosurfaces. Users may also specify triangle side length bounds in manually defined regions of the isosurface. 

Users can choose between 6 initial tilings to initiate the mesh generation with. Examples of each are shown below, each with different angle distribution results. Note that each bar of the histogram represents a window of 10 degrees, and the bars coloured red occur at a frequency *significantly* higher than the values represented in grey.

### 1) Uniform Isosceles Tiling
<img width="983" alt="Screen Shot 2022-12-15 at 8 57 22 PM" src="https://user-images.githubusercontent.com/54902342/208004495-78b5fc58-7418-4aca-a8c8-1bbf13e91029.png">

### 2) Uniform Equilateral Tiling
<img width="977" alt="Screen Shot 2022-12-15 at 8 57 41 PM" src="https://user-images.githubusercontent.com/54902342/208004496-0159e638-c411-4e8b-8647-ef7bafa10d5a.png">

### 3) Adaptive Isosceles-Single Tiling
<img width="977" alt="Screen Shot 2022-12-15 at 8 55 10 PM" src="https://user-images.githubusercontent.com/54902342/208004489-0373b375-c040-4aab-9a5e-de1f7a6587e1.png">

### 4) Adaptive Isosceles-Double Tiling
<img width="982" alt="Screen Shot 2022-12-15 at 8 55 41 PM" src="https://user-images.githubusercontent.com/54902342/208004491-bd03a766-25a6-467b-9414-ea9048151b06.png">

### 5) Adaptive Equilateral Tiling
<img width="986" alt="Screen Shot 2022-12-15 at 8 56 12 PM" src="https://user-images.githubusercontent.com/54902342/208004492-1b348730-a0f3-4343-907c-429d579a219f.png">

### 6) Adaptive All-Acute Tiling
<img width="983" alt="Screen Shot 2022-12-15 at 8 56 36 PM" src="https://user-images.githubusercontent.com/54902342/208004494-31de78b3-aca9-4b70-95bf-fa7a354f3b57.png">


## Build Steps
Prerequisites: C++, CMake

1. Clone repository
2. `mkdir build` in root directory 
3. `cd build`
4. `cmake ..`
5. `make`

This will create a .dylib in the build folder. Any application with access to this library, as well as the appropriate search path to the include folder of the cloned repo will be able to `#include <meshGeneration.hpp>`

## API
The library contains one main function with 2 overloaded versions: 

### ```pair<vector<pair<float, float>>, vector<vector<int>>> generateMesh(...)```

- Overload 1 allows the generated mesh to refine around the isosurface boundary (if passed an adaptive TilingType).
- Overload 2 allows the generated mesh to refine around user-defined regions.
 
### Function Overload 1: Isosurface Refinement
```
pair<vector<pair<float, float>>, vector<vector<int>>> generateMesh(float (*signedDistanceFunction)(float, float), float isosurfaceWidth, float isosurfaceHeight, float originX, float originY, TilingType tilingType, float minimumTriangleSideLength)
```

Inputs:
- `float (*signedDistanceFunction)(float, float)`: The signed distance function that represents the isosurface
- `float isosurfaceWidth`: The width of the bounding box that contains the isosurface
- `float isosurfaceHeight`: The height of the bounding box that contains the isosurface
- `float originX`: The x-Value of the bottom left corner of the bounding box that contains the isosurface
- `float originY`: The y-Value of the bottom left corner of the bounding box that contains the isosurface
- `TilingType tilingType`: The key for the desired tiling type (one of `{ ISOSCELES, EQUILATERAL, EQUILATERAL_ADAPTIVE, ISOSCELES_SINGLE_ADAPTIVE, ISOSCELES_DOUBLE_ADAPTIVE, ALL_ACUTE_ADAPTIVE }`)
- `float minimumTriangleSideLength`: The minimum side length of any triangle in the initial tiling. If the tilingType is uniform, this minimum value is applied throughout the entire tiling. If the tilingType is adaptive, this minimum value is employed in regions of refinement; at the isosurface boundary.

### Function Overload 2: Manual Refinement
```
pair<vector<pair<float, float>>, vector<vector<int>>> generateMesh(float (*signedDistanceFunction)(float, float), float isosurfaceWidth, float isosurfaceHeight, float originX, float originY, TilingType tilingType, float (*sizingFunction)(float, float), float maxSizingRateOfChange)
```

Inputs:
- `float (*signedDistanceFunction)(float, float)`: The signed distance function that represents the isosurface
- `float isosurfaceWidth`: The width of the bounding box that contains the isosurface
- `float isosurfaceHeight`: The height of the bounding box that contains the isosurface
- `float originX`: The x-Value of the bottom left corner of the bounding box that contains the isosurface
- `float originY`: The y-Value of the bottom left corner of the bounding box that contains the isosurface
- `TilingType tilingType`: The key for the desired tiling type (one of `{ ISOSCELES, EQUILATERAL, EQUILATERAL_ADAPTIVE, ISOSCELES_SINGLE_ADAPTIVE, ISOSCELES_DOUBLE_ADAPTIVE, ALL_ACUTE_ADAPTIVE }`)
- `float (*sizingFunction)(float, float)`: The sizing function. Returns the maximum allowed triangle side length for every region of the isosurface
- `float maxSizingRateOfChange`: The maximum rate of change of the sizing function. Can be interpreted as the smallest dimension of any region specified in the sizing function.

### Output
The output is a pair
  - The first value is a vector of float pairs, representing the list of vertices that compose the resulting mesh
  - The second value is a vector of integer triplets, where each triplet represents a traingle in the resulting mesh (note that each triplet is a vector with fixed size 3). Each integer value in the triplet points to a unique element in the list of vertices.

## Example
![Screen Shot 2022-12-15 at 9 21 24 PM](https://user-images.githubusercontent.com/54902342/208007189-a58e0572-4498-4f44-aed5-bf93ce957c40.png)


### Example 1: Isosurface Refinement (Overload 1)

Here we have a circle with radius 200, centered at (0, 0).

This can be modelled with the following signed distance function:
```
float signedDistanceFunction(float x, float y) {
    float radius = 200;
    float vectorLength = sqrt((x)*(x) + (y)*(y));
    return vectorLength - radius;
}
```
Note that if the circle were to be transformed (translated, rotated, etc), this would have to be reflected in the signed distance function as well.

</br>

By inspection, we can also obtain the following parameters (properties of the bounding box, in blue):
- `isosurfaceWidth = 500`
- `isosurfaceHeight = 500`
- `originX = -250`
- `originY = -250`

</br>

The tilingType is chosen by the user. Say the desired tiling is Adaptive All-Acute. As such, `tilingType = ALL_ACUTE_ADAPTIVE`.

The minimumTriangleSideLength is also chosen by the user. Say the desired minimum threshold is 10. As such, `minimumTriangleSideLength = 10`.

</br>

This leaves us with the following function call:
```
pair<vector<pair<float, float>>, vector<vector<int>>> output;
output = generateMesh(&signedDistanceFunction, 500, 500, -250, -250, ALL_ACUTE_ADAPTIVE, 10);
```

### Example 2: Manual Refinement (Overload 2)
The inputs will be the exact same as in example 1, except we will now provide both the `float (*sizingFunction)(float, float)` and `float maxSizingRateOfChange`, rather than the original final parameter, `minimumTriangleSideLength`.

![Screen Shot 2022-12-15 at 9 40 49 PM](https://user-images.githubusercontent.com/54902342/208009663-420130df-c33d-4c45-b552-a02d32e7657d.png)

Suppose that we want the maximum allowed triangle side lengths to be modelled as follows:
- Green region: 80
- Yellow Region: 25
- Blue Region: 10
- Elsewhere: 100

From this information, the sizing function would look something like:
```
float sizingFunction(float x, float y) {
    if (x >= -100 && x <= 0 && y >= 0 && y <= 100) return 80; // Large green square
    if (x >= -100 && x <= -80 && y >= 100 && y <= 120) return 80; // Small green square 
    if (x >= -60 && x <= 20 && y >= -60 && y <= 0) return 25; // Yellow rectangle
    if (x >= 0 && x <= 50 && y >= 0 && y <= 50) return 10; // Blue square
    return 100;
}
```

</br>


Remember that `float maxSizingRateOfChange` can be interpreted as the minimum dimension in any region of unique refinement. This means that the width of the small green square, 20, is the value representing the `maxRateOfChange`. 


This leaves us with the following function call:
```
pair<vector<pair<float, float>>, vector<vector<int>>> output;
output = generateMesh(&signedDistanceFunction, 500, 500, -250, -250, ALL_ACUTE_ADAPTIVE, &sizingFunction, 20);
```

## Conclusion

Created by Rayhan Moidu under the supervision of Professor Christopher Batty. 
Computational Motion Group, University of Waterloo.

Sources: 
- http://graphics.berkeley.edu/papers/Labelle-ISF-2007-07/Labelle-ISF-2007-07.pdf
- https://www.ics.uci.edu/~eppstein/pubs/BerEppGil-JCSS-94.pdf

Thanks for reading!

