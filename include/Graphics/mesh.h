// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _MESH_H_
#define _MESH_H_ 1

#include <vector>

#include "scoped_array.h"
#include "Core/platform_types.h"
#include <vector>

typedef u32 MeshHandler;

namespace Aloe {

  class Mesh {
  public:
    Mesh();
    ~Mesh();

    /** Sets the position of the mesh given a float* array and a size*/
    void setPositions(const float* v, u32 size);
    /** Sets the normals of the mesh given a float* array and a size*/
    void setNormals(const float* v, u32 size);
    /** Sets the uvs of the mesh given a float* array and a size*/
    void setUvs(const float* v, u32 size);
    /** Sets the indices of the mesh given a u32* array and a size*/
    void setIndices(const u32* i, u32 size);

    /** Gets the position array*/
    scoped_array<float>* getPositions();
    /** Gets the normals array*/
    scoped_array<float>* getNormals();
    /** Gets the uvs array*/
    scoped_array<float>* getUvs();
    /** Gets the indices array*/
    scoped_array<u32>* getIndices();

    /** Loads an obj*/
    void loadOBJ(const char* path);

    /** Internal mesh handler getter*/
    MeshHandler getHandler();

  private:
    //Internal Data
    struct Data;
    Data* data;

  };

  /** Global helper function to create a cube*/
  Mesh* CreateCubeMesh();
  /** Global helper function to create a plane*/
  Mesh* CreatePlaneMesh(u32 size_x,u32 size_z);
  /** Global helper function to create a terrain*/
  Mesh* CreateTerrainMesh(u32 size_x, u32 size_y);
  /** Global helper function to create a grass mesh*/
  Mesh* CreateGrassMesh(Mesh* terrain, float height);
}
#endif