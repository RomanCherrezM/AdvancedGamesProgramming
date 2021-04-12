// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Graphics/mesh.h"

#include "Core/engine.h"
#include "tiny_obj_loader.h"

namespace Aloe {

  struct Mesh::Data {
    MeshHandler handler;
  };

  Mesh::Mesh(){
    data = new Data;
    data->handler = -1;
  }

  Mesh::~Mesh() {
  }

  void Mesh::setPositions(const float* v, u32 size){
    ResourceManager* r = &Engine::get()->resource_manager;

    if (data->handler == -1) {
      data->handler = r->newMesh();
    }
    r->setMeshPositions(data->handler, v,size);
  }

  void Mesh::setNormals(const float* v, u32 size) {
    ResourceManager* r = &Engine::get()->resource_manager;

    if (data->handler == -1) {
      data->handler = r->newMesh();
    }
    r->setMeshNormals(data->handler, v,size);
  }

  void Mesh::setUvs(const float* v, u32 size) {
    ResourceManager* r = &Engine::get()->resource_manager;

    if (data->handler == -1) {
      data->handler = r->newMesh();
    }
    r->setMeshUvs(data->handler, v,size);
  }

  void Mesh::setIndices(const u32* i, u32 size){
    ResourceManager* r = &Engine::get()->resource_manager;

    if (data->handler == -1) {
      data->handler = r->newMesh();
    }
    r->setMeshIndices(data->handler, i,size);
  }

  scoped_array<float>* Mesh::getPositions(){
    ResourceManager* r = &Engine::get()->resource_manager;
    return &r->meshes[data->handler].positions;
  }
  scoped_array<float>* Mesh::getNormals() {
    ResourceManager* r = &Engine::get()->resource_manager;
    return &r->meshes[data->handler].normals;
  }
  scoped_array<float>* Mesh::getUvs() {
    ResourceManager* r = &Engine::get()->resource_manager;
    return &r->meshes[data->handler].uvs;
  }
  scoped_array<u32>* Mesh::getIndices() {
    ResourceManager* r = &Engine::get()->resource_manager;
    return &r->meshes[data->handler].indices;
  }

  void Mesh::loadOBJ(const char* path) {
  
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string err;

    if (tinyobj::LoadObj(shapes, material, err, path)) {    

      scoped_array<float> positions;
      scoped_array<float> normals;
      scoped_array<float> uvs;
      scoped_array<u32> indices;

      // Check total sizes of positions, normals, uvs and indices
      int pos_size_total = 0;
      int nor_size_total = 0;
      int uvs_size_total = 0;
      int ind_size_total = 0;

      for each (auto shape in shapes){
        pos_size_total += shape.mesh.positions.size();
        nor_size_total += shape.mesh.normals.size();
        uvs_size_total += shape.mesh.texcoords.size();
        ind_size_total += shape.mesh.indices.size();
      }

      positions.alloc(pos_size_total);
      normals.alloc(nor_size_total);
      uvs.alloc(uvs_size_total);
      indices.alloc(ind_size_total);

      // Offsets to concat 
      unsigned int pos_offset = 0;
      unsigned int nor_offset = 0;
      unsigned int uvs_offset = 0;
      unsigned int ind_offset = 0;
      unsigned int ind_factor = 0;

      // Concat vertex data and index data
      for each (auto shape in shapes) {
        
        unsigned int pos_size = shape.mesh.positions.size();
        unsigned int nor_size = shape.mesh.normals.size();
        unsigned int uvs_size = shape.mesh.texcoords.size();
        unsigned int ind_size = shape.mesh.indices.size();

        // Concat
        if (pos_size != 0)positions.memCopy(pos_offset ,&shape.mesh.positions[0], pos_size);
        if (nor_size != 0)normals.memCopy(nor_offset ,&shape.mesh.normals[0], nor_size);
        if (uvs_size != 0)uvs.memCopy(uvs_offset ,&shape.mesh.texcoords[0], uvs_size);

        // Fix indices
        for (int j = 0; j < shape.mesh.indices.size(); ++j) {
          shape.mesh.indices[j] += ind_factor;
        }

        if (ind_size != 0)indices.memCopy(ind_offset,&shape.mesh.indices[0], ind_size);

        pos_offset += pos_size;
        nor_offset += nor_size;
        uvs_offset += uvs_size;
        ind_offset += ind_size;
        ind_factor += shape.mesh.positions.size() / 3;
      }   

      setPositions(&positions[0],pos_size_total);
      setNormals(&normals[0], nor_size_total);
      setUvs(&uvs[0], uvs_size_total);
      setIndices(&indices[0], ind_size_total);

    }  
  }

  MeshHandler Mesh::getHandler(){
    return data->handler;
  }

  //Helper functions ------------------------------------------------------------------

  // Cube 
  Mesh* CreateCubeMesh() {
    float cube_positions[] = {
      //TOP FACE
      -0.5f,  0.5f,  0.5f,			//0
      0.5f, 0.5f,  0.5f,				//1
      0.5f, 0.5f,  -0.5f,				//2
      -0.5f, 0.5f,  -0.5f,			//3

      //BACK FACE
      -0.5f, 0.5f,  -0.5f,			//4
      0.5f, 0.5f,  -0.5f,				//5
      0.5f, -0.5f,  -0.5f,			//6
      -0.5f,  -0.5f,  -0.5f,		//7

      //RIGHT FACE
      0.5f,  0.5f,  -0.5f,			//8
      0.5f,  0.5f,  0.5f,				//9
      0.5f,  -0.5f,  0.5f,			//10
      0.5f, -0.5f,  -0.5f,			//11

      //LEFT FACE
      -0.5f, 0.5f, 0.5f,				//12
      -0.5f, 0.5f, -0.5f,				//13
      -0.5f, -0.5f, -0.5f,			//14
      -0.5f, -0.5f, 0.5f,				//15

      //FRONT FACE
      0.5f, 0.5f, 0.5f,					//16
      -0.5f, 0.5f, 0.5f,				//17
      -0.5f,  -0.5f, 0.5f,			//18
      0.5f,  -0.5f, 0.5f,				//19

      //BOTTOM FACE
      0.5f,  -0.5f, -0.5f,			//20
      -0.5f,  -0.5f, -0.5f,			//21
      -0.5f,  -0.5f, 0.5f,			//22
      0.5f,  -0.5f, 0.5f 			//23

    };
    float cube_normals[] = {
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,

      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,

      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,

      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,

      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,

      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f };
    float cube_uvs[] = {
      0.0f,0.0f,
      1.0f,0.0f,
      1.0f,1.0f,
      0.0f,1.0f,

      0.0f,0.0f,
      1.0f,0.0f,
      1.0f,1.0f,
      0.0f,1.0f,

      0.0f,0.0f,
      1.0f,0.0f,
      1.0f,1.0f,
      0.0f,1.0f,

      0.0f,0.0f,
      1.0f,0.0f,
      1.0f,1.0f,
      0.0f,1.0f,

      0.0f,0.0f,
      1.0f,0.0f,
      1.0f,1.0f,
      0.0f,1.0f,

      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f };
    unsigned int cube_indices[] = {
    0,1,2,0,2,3,
    4,5,6,4,6,7,
    8,9,10,8,10,11,
    12,13,14,12,14,15,
    16,17,18,16,18,19,
    20,22,21,20,23,22 };

    Mesh* mesh = new Mesh;
    mesh->setPositions(cube_positions,sizeof(cube_positions) /  sizeof(float));
    mesh->setNormals(cube_normals,sizeof(cube_normals) /  sizeof(float));
    mesh->setUvs(cube_uvs,sizeof(cube_uvs) /  sizeof(float));
    mesh->setIndices(cube_indices,sizeof(cube_indices) /  sizeof(unsigned int));

    return mesh;
  }

  // Plane
  struct Plane {
    scoped_array<float> positions;
    scoped_array<float> normals;
    scoped_array<float> uvs;
    scoped_array<u32> indices;
  };
  void CreatePlane(Plane* p,u32 vertices_x, u32 vertices_z) {

    p->positions.alloc(vertices_x * vertices_z * 3);
    p->normals.alloc(vertices_x * vertices_z * 3);
    p->uvs.alloc(vertices_x * vertices_z * 2);
    p->indices.alloc((vertices_x - 1) * (vertices_z - 1) * 6);

    int vertex_i = 0;
    int uvs_i = 0;
    int ind_i = 0;
    //Generate vertices
    for (int i = 0; i < vertices_z; ++i) {
      for (int j = 0; j < vertices_x; ++j) {
        // Positions
        p->positions[vertex_i + 0] = (float)j / ((float)vertices_x);
        p->positions[vertex_i + 1] = 0.0f;
        p->positions[vertex_i + 2] = (float)i / ((float)vertices_z);

        //Normals
        p->normals[vertex_i + 0] = 0.0f;				
        p->normals[vertex_i + 1] = 1.0f;		
        p->normals[vertex_i + 2] = 0.0f;		
        
        //Uvs
        p->uvs[uvs_i] = ((float)j) / ((float)vertices_x);		  
        p->uvs[uvs_i + 1] = ((float)i) / ((float)vertices_z);	

        vertex_i += 3;
        uvs_i += 2;

        //Indices
        if (i < vertices_z - 1) {
          if (j < vertices_x - 1) {
            int offset = j + (i * vertices_z);

            p->indices[ind_i + 0] = offset;
            p->indices[ind_i + 1] = offset + vertices_x;
            p->indices[ind_i + 2] = offset + 1;

            p->indices[ind_i + 3] = offset + vertices_x;
            p->indices[ind_i + 4] = offset + vertices_x + 1;
            p->indices[ind_i + 5] = offset + 1;

            ind_i += 6;
          }
        }
      }
    }
  }
  Mesh* CreatePlaneMesh(u32 size_x, u32 size_z) {

    Mesh* plane_mesh;
    plane_mesh = new Mesh;

    Plane p; 
    CreatePlane(&p, size_x, size_z);

    plane_mesh->setPositions(&p.positions[0], p.positions.size());
    plane_mesh->setNormals(&p.normals[0], p.normals.size());
    plane_mesh->setUvs(&p.uvs[0], p.uvs.size());
    plane_mesh->setIndices(&p.indices[0], p.indices.size());

    return plane_mesh;
  }

  // Terrain 
  struct Noise {
    scoped_array<float> data;
  };
  // Using only 2^n + 1 numbers as size

  // Perlin Noise
  static int hash[] = { 208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
                     185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
                     9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
                     70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
                     203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
                     164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
                     228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
                     232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
                     193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
                     101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
                     135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
                     114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219 };
  static int inline noise2(int x, int y,int seed){
    int tmp = hash[(y + seed) % 256];
    return hash[(tmp + x) % 256];
  }
  static float inline lin_inter(float x, float y, float s){
    return x + s * (y - x);
  }
  static float inline smooth_inter(float x, float y, float s){
    return lin_inter(x, y, s * s * (3 - 2 * s));
  }
  
  // 
  void DiamondSquareNoise(Noise* noise,u32 size) {
  
    noise->data.alloc((size + 1) * (size + 1));
    noise->data.memSet(0);

    //Set to random values
    int r = rand() % 256;
    noise->data[0] = r;                           // Top left
    noise->data[size] = r;                        // Top right
    noise->data[(size + 1) * size] = r;           // Bot left
    noise->data[(size + 1) * (size + 1) - 1] = r; // Bot right

    // The more the rougher
    int random_factor = 128;
    int tile_size = size;

    while (tile_size > 1) {
      int half = floor(tile_size / 2);

      for (int x = 0; x < size; x += tile_size) {
        for (int y = 0; y < size; y += tile_size) {
        
          float sum =
            noise->data[y * (size + 1) + x] +
            noise->data[y * (size + 1) + (x + tile_size)] +
            noise->data[(y + tile_size) * (size + 1) + x] +
            noise->data[(y + tile_size) * (size + 1) + (x + tile_size)];

          float avg = sum / 4.0f;
          avg += -random_factor + rand() % (random_factor*2);

          noise->data[(y + half) * (size + 1) + (x + half)] = avg;
        } 
      }

      for (int x = 0; x < size; x += half) {
        for (int y = ((x+half) % tile_size); y < size; y += tile_size) {

          float sum =
            noise->data[y * (size + 1) + (((x - half + size) % (size)))] +
            noise->data[y * (size + 1) + (((x + half) % (size)))] +
            noise->data[(((y + half) % (size))) * (size + 1) + x] +
            noise->data[(((y - half + size) % (size))) * (size + 1) + x];

          float avg = sum / 4.0f;
          avg += -random_factor + rand() % (random_factor * 2);

          noise->data[y * (size + 1) + x] = avg;

          if (x == 0) noise->data[y * (size + 1) + (x + size)] = avg;
          if (y == 0) noise->data[(y+size) * (size + 1) + x] = avg;
        }
      }

      random_factor = glm::max(random_factor/2,1);
      tile_size = floor(tile_size/2);
    }

    //Print
    /*printf("\n");
    for (int y = 0; y < size + 1;++y) {
      for (int x = 0; x < size + 1; ++x) {
        printf("%4d ", (int)noise->data[y * (size +1) + x]);
      }
      printf("\n");
    }*/

  }
  float PerlinNoise2D(float x, float y, float freq, int depth,int seed){
    float xa = x * freq;
    float ya = y * freq;
    float amp = 1.0;
    float fin = 0;
    float div = 0.0;

    int i;
    for (i = 0; i < depth; i++){
      div += 256 * amp;

      int x_int = xa;
      int y_int = ya;
      float x_frac = xa - x_int;
      float y_frac = ya - y_int;
      int s = noise2(x_int, y_int,seed);
      int t = noise2(x_int + 1, y_int,seed);
      int u = noise2(x_int, y_int + 1,seed);
      int v = noise2(x_int + 1, y_int + 1,seed);
      float low = smooth_inter(s, t, x_frac);
      float high = smooth_inter(u, v, x_frac);
      float smooth = smooth_inter(low, high, y_frac);

      fin += smooth * amp;
      amp /= 2;
      xa *= 2;
      ya *= 2;
    }

    return fin / div;
  }

  Mesh* CreateTerrainMesh(u32 size_x,u32 size_y) {

    Mesh* plane_mesh;
    plane_mesh = new Mesh;

    Plane p;
    CreatePlane(&p, size_x, size_y);

    // Elevate terrain
    for (int y = 0; y < size_y*3;y+=3) {
      for (int x = 0; x < size_x*3; x+=3) {
        p.positions[y * size_y + x +1] = PerlinNoise2D(x,y, 0.003, 2,7);
      }
    }

    // Calculate normals from height
    for (int y = 0; y < size_y * 3; y += 3) {
      for (int x = 0; x < size_x * 3; x += 3) {
        float left  = PerlinNoise2D(x - 1, y, 0.003, 2, 7);
        float right = PerlinNoise2D(x + 1, y, 0.003, 2, 7);
        float down  = PerlinNoise2D(x, y - 1, 0.003, 2, 7);
        float up    = PerlinNoise2D(x, y + 1, 0.003, 2, 7);

        glm::vec3 normal(0.0f);
        normal.x = right - left;
        normal.y = 0.005f;
        normal.z = up - down;
        normal = glm::normalize(normal);

        p.normals[y * size_y + x] = normal.x;
        p.normals[y * size_y + x + 1] = normal.y;
        p.normals[y * size_y + x + 2] = normal.z;
      }
    }

    plane_mesh->setPositions(&p.positions[0], p.positions.size());
    plane_mesh->setNormals(&p.normals[0], p.normals.size());
    plane_mesh->setUvs(&p.uvs[0], p.uvs.size());
    plane_mesh->setIndices(&p.indices[0], p.indices.size());

    return plane_mesh;
  }

  Mesh* CreateGrassMesh(Mesh* terrain, float height) {
  
    Mesh* grass;
    grass = new Mesh;

    scoped_array<float> *positions = terrain->getPositions();
    scoped_array<float> *normals = terrain->getNormals();

    int count = 0;
    std::vector<u32> aux;
    for (int i = 0; i < positions->size(); i+=3) {
      if (positions->get()[i + 1] >= height) {
        aux.push_back(i);
        count++;
      }
    }

    if (count == 0)printf("\nHEIGHT NOT VALID");

    scoped_array<float> grass_pos;
    scoped_array<u32> grass_ind;

    grass_pos.alloc(count * 3);
    grass_ind.alloc(count);

    for (int i = 0; i < grass_pos.size() / 3; i++) {
      grass_pos[i * 3 + 0] = positions->get()[aux[i] + 3];
      grass_pos[i * 3 + 1] = positions->get()[aux[i] + 1];
      grass_pos[i * 3 + 2] = positions->get()[aux[i] + 2];

      grass_ind[i] = i;
    }

    grass->setPositions(&grass_pos[0],grass_pos.size());
    grass->setIndices(&grass_ind[0],grass_ind.size());

    return grass;
  }

}