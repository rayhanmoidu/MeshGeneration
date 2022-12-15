#include "Algorithm.hpp"
#include "Tiling.hpp"
#include "Isosurface.hpp"
#include "Point.hpp"
#include <iostream>
#include <cmath>

Algorithm::Algorithm(Tiling *baseTiling, Isosurface &isosurface, float alphaVal) : baseTiling(baseTiling), isosurface(isosurface) {
    allTriangles = baseTiling->getTriangles();
    alpha = alphaVal;
}

void Algorithm::execute() {
    findTrianglesOverlayingIsosurface();
    findCutPoints();
    warpPerimeterTriangles();
    clipPerimeterTriangles();
}

void Algorithm::findTrianglesOverlayingIsosurface() {
    for (int i = 0; i < allTriangles.size(); i++) {
        Triangle curTriangle = allTriangles[i];
        vector<Point> trianglePoints = curTriangle.getPoints();
        int point1Value = isosurface.evaluatePoint(trianglePoints[0]);
        int point2Value = isosurface.evaluatePoint(trianglePoints[1]);
        int point3Value = isosurface.evaluatePoint(trianglePoints[2]);
        
        if (point1Value==1 || point2Value==1 || point3Value==1) {
            processedTriangles.push_back(curTriangle);
        }
    }
}

Point Algorithm::interpolateCutpoint(Point positiveVertex, Point negativeVertex) {
    Point potentialCutpoint = positiveVertex;
    while (1) {
        potentialCutpoint = findCutpointLocationBetweenPolarPoints(potentialCutpoint, negativeVertex);
        if (abs(isosurface.signedDistanceFunction(potentialCutpoint)) < 0.5) break;
    }
    return potentialCutpoint;
}

Point Algorithm::findCutpointLocationBetweenPolarPoints(Point positiveVertex, Point negativeVertex) {
    float dx = positiveVertex.getX() - negativeVertex.getX();
    float dy = positiveVertex.getY() - negativeVertex.getY();
    
    float negativeVertexIsosurfaceDistance = abs(isosurface.signedDistanceFunction(negativeVertex));
    float positiveVertexIsosurfaceDistance = abs(isosurface.signedDistanceFunction(positiveVertex));
    
    float cutpointPercentageAlongSimulatedLine = negativeVertexIsosurfaceDistance / (negativeVertexIsosurfaceDistance + positiveVertexIsosurfaceDistance);
    
    float cutpointX = negativeVertex.getX() + cutpointPercentageAlongSimulatedLine*dx;
    float cutpointY = negativeVertex.getY() + cutpointPercentageAlongSimulatedLine*dy;
    
    return Point(cutpointX, cutpointY);
}

void Algorithm::findCutPoints() {
    vector<Triangle*> perimeterTriangles = getPerimeterTrianglesForProcessing();
    
    for (int i = 0; i < perimeterTriangles.size(); i++) {
        Triangle *curTriangle = perimeterTriangles[i];
        vector<Point> trianglePoints = curTriangle->getPoints();
        vector<vector<Point>> slicedPoints = sliceTrianglePointsBySign(trianglePoints);
        
        vector<Point> positiveVertices = slicedPoints[0];
        vector<Point> negativeVertices = slicedPoints[1];
        
        for (int posIndex = 0; posIndex < positiveVertices.size(); posIndex++) {
            for (int negIndex = 0; negIndex < negativeVertices.size(); negIndex++) {
                
                Point cutpoint = interpolateCutpoint(positiveVertices[posIndex], negativeVertices[negIndex]);
                
                Cutpoint newCutpoint(cutpoint.getX(), cutpoint.getY(), positiveVertices[posIndex], negativeVertices[negIndex]);
                curTriangle->addCutpoint(newCutpoint);
            }
        }
    }
}

void Algorithm::warpPerimeterTriangles() {
    vector<Triangle*> perimeterTriangles = getPerimeterTrianglesForProcessing();
    
    for (int i = 0; i < perimeterTriangles.size(); i++) {
        Triangle *curTriangle = perimeterTriangles[i];
        vector<Point> trianglePoints = curTriangle->getPoints();
        
        for (int j = 0; j < trianglePoints.size(); j++) {
            Point curPoint = trianglePoints[j];
            
            // only warp negative vertices
            if (isosurface.evaluatePoint(curPoint)==-1) {
                
                // find all triangles sharing the negative vertex
                vector<Triangle*> trianglesSharingVertex;
                for (int k = 0; k < perimeterTriangles.size(); k++) {
                    int lala = baseTiling->check_addVertex_getIndex(curPoint); // added
                    if (perimeterTriangles[k]->doesContainVertex(curPoint) || perimeterTriangles[k]->doesContainIndex(lala)) {
                        trianglesSharingVertex.push_back(perimeterTriangles[k]);
                    }
                }
                
                // find all cutpoints bound by the negative vertex (must check all cp's of all triangles sharing negative vertex)
                vector<Cutpoint> cutpointsToConsider;
                for (int k = 0; k < trianglesSharingVertex.size(); k++) {
                    vector<Cutpoint> potentialCutpointsToConsider = trianglesSharingVertex[k]->getCutpoints();
                    for (int l = 0; l < potentialCutpointsToConsider.size(); l++) {
                        if (potentialCutpointsToConsider[l].isContainedByVertex(curPoint)) {
                            cutpointsToConsider.push_back(potentialCutpointsToConsider[l]);
                        }
                    }
                }
                
                // find cutpoint with smallest distance from negative vertex
                float lowestDistance = MAXFLOAT;
                int lowestDistanceIndex = -1;
                for (int k = 0; k < cutpointsToConsider.size(); k++) {
                    if (distanceBetweenTwoPoints(curPoint, cutpointsToConsider[k]) < lowestDistance) {
                        lowestDistance = distanceBetweenTwoPoints(curPoint, cutpointsToConsider[k]);
                        lowestDistanceIndex = k;
                    }
                }
                
                // if the distance is less than alpha, warp negative vertex to cutpoint
                if (lowestDistanceIndex != -1 && lowestDistance < alpha) {
                    Cutpoint warpingDestination = cutpointsToConsider[lowestDistanceIndex];
                    
                    for (int k = 0; k < trianglesSharingVertex.size(); k++) {
                        int warpingDestinationIndex = baseTiling->check_addVertex_getIndex(warpingDestination);
                        trianglesSharingVertex[k]->warpVertexToCutpoint(curPoint, warpingDestination, warpingDestinationIndex);
                        // add warping destination to list of vertices and adjust triangles accordingly
                    }
                    
                    // get rid of all cutpoints bound by the negative vertex
                    for (int k = 0; k < cutpointsToConsider.size(); k++) {
                        for (int l = 0; l < perimeterTriangles.size(); l++) {
                            if (perimeterTriangles[l]->doesContainCutpoint(cutpointsToConsider[k])) {
                                perimeterTriangles[l]->removeCutpoint(cutpointsToConsider[k]);
                            }
                        }
                    }
                }
            }
        }
    }
}

void Algorithm::clipPerimeterTriangles() {
    vector<Triangle*> perimeterTriangles = getPerimeterTrianglesForProcessing();
    
    for (int i = 0; i < perimeterTriangles.size(); i++) {
        Triangle *curTriangle = perimeterTriangles[i];
        vector<Point> trianglePoints = curTriangle->getPoints();
        vector<vector<Point>> slicedPoints = sliceTrianglePointsBySign(trianglePoints);
        
        vector<Point> positiveVertices = slicedPoints[0];
        vector<Point> negativeVertices = slicedPoints[1];
        
        if (negativeVertices.size()>=1) {
            if (negativeVertices.size()==2) {
                Point p1 = interpolateCutpoint(positiveVertices[0], negativeVertices[0]);
                Cutpoint cp1(p1.getX(), p1.getY(), positiveVertices[0], negativeVertices[0]);
                Point p2 = interpolateCutpoint(positiveVertices[0], negativeVertices[1]);
                Cutpoint cp2(p2.getX(), p2.getY(), positiveVertices[0], negativeVertices[1]);
                
                int cp1Index = baseTiling->check_addVertex_getIndex(cp1);
                int cp2Index = baseTiling->check_addVertex_getIndex(cp2);
                int posV0Index = baseTiling->check_addVertex_getIndex(positiveVertices[0]);
                
                Triangle newTriangle = Triangle(cp1, cp2, positiveVertices[0], cp1Index, cp2Index, posV0Index);
                processedTriangles.push_back(newTriangle);
            } else if (negativeVertices.size()==1) {
                Point p1 = interpolateCutpoint(positiveVertices[0], negativeVertices[0]);
                Cutpoint cp1(p1.getX(), p1.getY(), positiveVertices[0], negativeVertices[0]);
                Point p2 = interpolateCutpoint(positiveVertices[1], negativeVertices[0]);
                Cutpoint cp2(p2.getX(), p2.getY(), positiveVertices[1], negativeVertices[0]);
                
                int cp1Index = baseTiling->check_addVertex_getIndex(cp1);
                int cp2Index = baseTiling->check_addVertex_getIndex(cp2);
                int posV0Index = baseTiling->check_addVertex_getIndex(positiveVertices[0]);
                int posV1Index = baseTiling->check_addVertex_getIndex(positiveVertices[1]);
                
                Triangle newTriangle1 = Triangle(cp1, positiveVertices[1], positiveVertices[0], cp1Index, posV1Index, posV0Index);
                Triangle newTriangle2 = Triangle(cp1, cp2, positiveVertices[1], cp1Index, cp2Index, posV1Index);
                
                // find angle at posvertex0 and cp2
                
                if (doesPassDelaunaysCondition(cp1, cp2, positiveVertices[0], positiveVertices[1])) {
                    processedTriangles.push_back(newTriangle1);
                    processedTriangles.push_back(newTriangle2);
                } else {
                    newTriangle1 = Triangle(cp1, cp2, positiveVertices[0], cp1Index, cp2Index, posV0Index);
                    newTriangle2 = Triangle(positiveVertices[1], cp2, positiveVertices[0], posV1Index, cp2Index, posV0Index);
                    processedTriangles.push_back(newTriangle1);
                    processedTriangles.push_back(newTriangle2);
                }
            }
        }
    }
    filterOutNegativeTriangles();
}

bool Algorithm::doesPassDelaunaysCondition(Point cp1, Point cp2, Point pos1, Point pos2) {
    float dx1 = cp1.getX() - cp2.getX();
    float dy1 = cp1.getY() - cp2.getY();
    float b1 = sqrt(dx1*dx1 + dy1*dy1);
    
    float dx2 = cp2.getX() - pos2.getX();
    float dy2 = cp2.getY() - pos2.getY();
    float c1 = sqrt(dx2*dx2 + dy2*dy2);
    
    float dx3 = cp1.getX() - pos1.getX();
    float dy3 = cp1.getY() - pos1.getY();
    float b2 = sqrt(dx3*dx3 + dy3*dy3);
    
    float dx4 = pos1.getX() - pos2.getX();
    float dy4 = pos1.getY() - pos2.getY();
    float c2 = sqrt(dx4*dx4 + dy4*dy4);
    
    float dx5 = cp1.getX() - pos2.getX();
    float dy5 = cp1.getY() - pos2.getY();
    float sharedSide = sqrt(dx5*dx5 + dy5*dy5);
    
    float angle1 = (acos((b1*b1 + c1*c1 - sharedSide*sharedSide) / (2*b1*c1))) * (180/M_PI);
    float angle2 = (acos((b2*b2 + c2*c2 - sharedSide*sharedSide) / (2*b2*c2))) * (180/M_PI);
    
    return (angle1 + angle2 <= 180);
}

void Algorithm::filterOutNegativeTriangles() {
    vector<Triangle> newProcessedTriangles;
    for (int i = 0; i < processedTriangles.size(); i++) {
        vector<Point> trianglePoints = processedTriangles[i].getPoints();
        if (isosurface.evaluatePoint(trianglePoints[0])!=-1 && isosurface.evaluatePoint(trianglePoints[1])!=-1 && isosurface.evaluatePoint(trianglePoints[2])!=-1) {
            newProcessedTriangles.push_back(processedTriangles[i]);
        }
    }
    processedTriangles = newProcessedTriangles;
}

vector<vector<Point>> Algorithm::sliceTrianglePointsBySign(vector<Point> trianglePoints) {
    int point1Value = isosurface.evaluatePoint(trianglePoints[0]);
    int point2Value = isosurface.evaluatePoint(trianglePoints[1]);
    int point3Value = isosurface.evaluatePoint(trianglePoints[2]);
    
    vector<Point> positiveVertices;
    vector<Point> negativeVertices;
    
    if (point1Value == -1) negativeVertices.push_back(trianglePoints[0]);
    if (point1Value == 1 || point1Value == 0) positiveVertices.push_back(trianglePoints[0]);
    if (point2Value == -1) negativeVertices.push_back(trianglePoints[1]);
    if (point2Value == 1 || point2Value == 0) positiveVertices.push_back(trianglePoints[1]);
    if (point3Value == -1) negativeVertices.push_back(trianglePoints[2]);
    if (point3Value == 1 || point3Value == 0) positiveVertices.push_back(trianglePoints[2]);
    
    vector<vector<Point>> slicedPoints;
    slicedPoints.push_back(positiveVertices);
    slicedPoints.push_back(negativeVertices);
    
    return slicedPoints;
}

vector<Triangle*> Algorithm::getPerimeterTrianglesForProcessing() {
    vector<Triangle*> perimeterTriangles;
    for (int i = 0; i < processedTriangles.size(); i++) {
        Triangle *curTriangle = &processedTriangles[i];
        vector<Point> trianglePoints = curTriangle->getPoints();
        int point1Value = isosurface.evaluatePoint(trianglePoints[0]);
        int point2Value = isosurface.evaluatePoint(trianglePoints[1]);
        int point3Value = isosurface.evaluatePoint(trianglePoints[2]);
        
        if (point1Value==-1 || point2Value==-1 || point3Value==-1) {
            perimeterTriangles.push_back(curTriangle);
        }

    }
    return perimeterTriangles;
}

float Algorithm::distanceBetweenTwoPoints(Point p1, Point p2) {
    float dx = p2.getX() - p1.getX();
    float dy = p2.getY() - p1.getY();
    return sqrt(dx*dx + dy*dy);
}

vector<vector<int>> Algorithm::getTriangleIndices() {
    vector<vector<int>> triangleIndices;
    for (int i = 0; i < processedTriangles.size(); i++) {
        triangleIndices.push_back(processedTriangles[i].getIndices());
    }
    return triangleIndices;
}

pair<vector<pair<float, float>>, vector<vector<int>>> Algorithm::getOutput(float originX, float originY) {
    vector<pair<float, float>> vertices = baseTiling->getVertices();
    vector<vector<int>> triangles = this->getTriangleIndices();
    
    std::unordered_map<int, int> oldIndexToNewIndex;
    std::unordered_set<int> coveredIndices;
    
    vector<pair<float, float>> retVertices;
    vector<vector<int>> retTriangles;
    
    for (int i = 0; i < triangles.size(); i++) {
        vector<int> oldTriangleIndices = triangles[i];
        vector<int> newTriangleIndices;
        
        for (int j = 0; j < oldTriangleIndices.size(); j++) {
            if (coveredIndices.count(oldTriangleIndices[j])) {
                newTriangleIndices.push_back(oldIndexToNewIndex.at(oldTriangleIndices[j]));
            } else {
                pair<float, float> oldVertex = vertices[oldTriangleIndices[j]];
                pair<float, float> newVertex(oldVertex.first + originX, oldVertex.second + originY);
                
                retVertices.push_back(newVertex);
                coveredIndices.insert(oldTriangleIndices[j]);
                
                pair<int, int> mapInsertion(oldTriangleIndices[j], retVertices.size()-1);
                oldIndexToNewIndex.insert(mapInsertion);
                
                newTriangleIndices.push_back(retVertices.size()-1);
            }
        }
        
        if (newTriangleIndices[0]!=newTriangleIndices[1] && newTriangleIndices[1]!=newTriangleIndices[2] && newTriangleIndices[2]!=newTriangleIndices[0]) {
            retTriangles.push_back(newTriangleIndices);
        }
    }
    
    pair<vector<pair<float, float>>, vector<vector<int>>> output(retVertices, retTriangles);
    return output;
}

