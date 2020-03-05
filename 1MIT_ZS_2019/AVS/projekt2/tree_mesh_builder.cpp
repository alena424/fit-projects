/**
 * @file    tree_mesh_builder.cpp
 *
 * @author  Alena Tesarova <xtesar36@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    8 December 2019, 21:00
 **/

#include <iostream>
#include <math.h>
#include <limits>

#include "tree_mesh_builder.h"

TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize)
    : BaseMeshBuilder(gridEdgeSize, "Octree")
{

}
/**
 * @param l - aktualni iso level
 * @param a - aktualni velikost hrany
 */
unsigned TreeMeshBuilder::marchCubesRecursive(float x, float y, float z, const ParametricScalarField &field, float a)
{
        float pom = (sqrt(3)/2);
        unsigned totalTriangles,totalTriangles_1,totalTriangles_2,totalTriangles_3,totalTriangles_4,totalTriangles_5,totalTriangles_6,totalTriangles_7,totalTriangles_8 = 0;
        float a_half = a/2;
         
         Vec3_t<float> pos((x+a_half)*mGridResolution,(y+a_half)*mGridResolution,(z+a_half)*mGridResolution); // realne hodnoty, stred kostky
         float distanceToClosestPoint = evaluateFieldAt(pos, field); //real
         
        if ( distanceToClosestPoint > ( mIsoLevel + (pom*a*mGridResolution) )){
                return 0;  // empty blok, no cubes
        } else if ( a > 1 ){
            
            /** SEKVENCNI RESENI PRO DANOU HLOUBKU CUT_OFF
            if (a <= 2){
                // sekvencni cast
                 for (int i = 0; i < a;i++){
                    for (int j = 0; j < a; j++){
                        for (int m = 0; m < a; m++){
                            float pomx = x + i;
                            float pomy = y + j;
                            float pomz = z + m;
                             Vec3_t<float> cubeOffset(pomx,pomy,pomz); 
                            totalTriangles += buildCube(cubeOffset, field);
                        }
                    }
                 }
            } else {}*/
            
              // ma smysl prohledavat
            #pragma omp task shared(totalTriangles_1) shared(totalTriangles)
            {
                totalTriangles_1 = marchCubesRecursive( x, y, z, field, a_half);
            
            }
            
            #pragma omp task shared(totalTriangles_2) shared(totalTriangles)
            {
                totalTriangles_2 = marchCubesRecursive( x+a_half, y, z, field, a_half);
            }
            
            #pragma omp task shared(totalTriangles_3) shared(totalTriangles)
            {
                totalTriangles_3  = marchCubesRecursive( x, y+a_half, z, field, a_half);
            }
            
            #pragma omp task shared(totalTriangles_4) shared(totalTriangles)
            {
                totalTriangles_4  = marchCubesRecursive( x, y, z+a_half, field, a_half);
            }
            
            #pragma omp task shared(totalTriangles_5) shared(totalTriangles)
            {
                totalTriangles_5  = marchCubesRecursive( x+a_half, y+a_half, z, field, a_half);
            }
            
            #pragma omp task shared(totalTriangles_6) shared(totalTriangles)
            {
                totalTriangles_6  = marchCubesRecursive( x+a_half, y, z+a_half, field, a_half);
            }
            
            #pragma omp task shared(totalTriangles_7) shared(totalTriangles)
            {
                totalTriangles_7  = marchCubesRecursive( x, y+a_half, z+a_half, field, a_half);
            }
            
            #pragma omp task shared(totalTriangles_8) shared(totalTriangles)
            {
                totalTriangles_8  = marchCubesRecursive( x+a_half, y+a_half, z+a_half, field, a_half);
            }
            
            #pragma omp taskwait
            return totalTriangles_8+totalTriangles_7+totalTriangles_6+totalTriangles_5+totalTriangles_4+totalTriangles_3+totalTriangles_2+totalTriangles_1;
            
            
        } else {
            Vec3_t<float> cubeOffset(x,y,z); //roh kostky - levy dolni
            return buildCube(cubeOffset, field);
           
        }
        
    
}

unsigned TreeMeshBuilder::marchCubes(const ParametricScalarField &field)
{
      unsigned totalTriangles = 0;
     #pragma omp parallel
	{
		#pragma omp single 
		{
        totalTriangles= marchCubesRecursive(0.0,0.0,0.0, field, mGridSize);
        }
    }
    return totalTriangles;
 
}

// F(P)
// budu mit bod, kolem neho udelam kouli a zjistim, jestli uvnitr je bod - zjistim, ze je to v kouli, kdyz to je v krychli, ale ne v kruhu / falsne pozitivni
float TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field)
{
     // NOTE: This method is called from "buildCube(...)"!

    // 1. Store pointer to and number of 3D points in the field
    //    (to avoid "data()" and "size()" call in the loop).
    const Vec3_t<float> *pPoints = field.getPoints().data();
    const unsigned count = unsigned(field.getPoints().size());

    float value = std::numeric_limits<float>::max();

    // 2. Find minimum square distance from points "pos" to any point in the
    //    field.
    for(unsigned i = 0; i < count; ++i)
    {
        float distanceSquared  = (pos.x - pPoints[i].x) * (pos.x - pPoints[i].x);
        distanceSquared       += (pos.y - pPoints[i].y) * (pos.y - pPoints[i].y);
        distanceSquared       += (pos.z - pPoints[i].z) * (pos.z - pPoints[i].z);

        // Comparing squares instead of real distance to avoid unnecessary
        // "sqrt"s in the loop.
        value = std::min(value, distanceSquared);
    }

    // 3. Finally take square root of the minimal square distance to get the real distance
    return sqrt(value);
}

void TreeMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle)
{
    #pragma omp critical
    {
     mTriangles.push_back(triangle);
    }
}
