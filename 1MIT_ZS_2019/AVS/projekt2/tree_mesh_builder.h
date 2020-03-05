/**
 * @file    tree_mesh_builder.h
 *
 * @author  Alena Tesarova <xtesar36@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    8 December 2019, 21:00
 **/

#ifndef TREE_MESH_BUILDER_H
#define TREE_MESH_BUILDER_H

#include "base_mesh_builder.h"

class TreeMeshBuilder : public BaseMeshBuilder
{
public:
    TreeMeshBuilder(unsigned gridEdgeSize);

    
protected:
    unsigned marchCubes(const ParametricScalarField &field);
    unsigned marchCubesRecursive(float x, float y, float z, const ParametricScalarField &field, float a);
    float evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field);
    void emitTriangle(const Triangle_t &triangle);
    const Triangle_t *getTrianglesArray() const { return mTriangles.data(); }
    
    std::vector<Triangle_t> mTriangles; 
};

#endif // TREE_MESH_BUILDER_H
