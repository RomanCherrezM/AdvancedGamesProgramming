// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_ 1

#include "Core/platform_types.h"

#include "Graphics/shader.h"
#include "Graphics/mesh.h"
#include "Graphics/Materials/base_material.h"
#include "Graphics/texture.h"
#include "Graphics/skybox.h"

#include "scoped_array.h"
#include <vector>

namespace Aloe {

  enum ResourceState {
    AVAILABLE = -1,
    TO_BE_CREATED,
    TO_BE_UPDATED,
    CREATED
  };

  struct InternalMesh {
    //OPENGL
    u32 vertex_gl_id;
    u32 index_gl_id;

    scoped_array<float> positions;
    scoped_array<float> normals;
    scoped_array<float> uvs;

    scoped_array<float> vertices;
    scoped_array<u32> indices;


    ResourceState state = AVAILABLE;
  };

  struct InternalTexture {
    //OPENGL
    u32 texture;

    u32 width;
    u32 height;
    u32 num_colors;

    Format format;
    Type type;
    Filter filter;
    Wrap wrap;

    scoped_array<unsigned char> data;

    ResourceState state = AVAILABLE;
  };

  struct InternalSkybox {

    SkyboxMaterial material;
    Mesh mesh;

  };

  /** The ResourceManager is the responsible for the creation of textures and meshes when needed,
  * it stores the data internally and creates the assets when needed
  */ 

  class ResourceManager {

  public:
    ResourceManager();
    ~ResourceManager();

    /** Creates a new mesh internally and returns a MeshHandler*/
    MeshHandler newMesh();
    /** Sets the mesh positions data of the MeshHandler*/
    void setMeshPositions(MeshHandler handler, const float* v, u32 size);
    /** Sets the mesh normals data of the MeshHandler*/
    void setMeshNormals(MeshHandler handler, const float* v, u32 size);
    /** Sets the mesh uvs data of the MeshHandler*/
    void setMeshUvs(MeshHandler handler, const float* v, u32 size);
    /** Sets the mesh indices data of the MeshHandler*/
    void setMeshIndices(MeshHandler handler, const u32* i, u32 size);
    /** Gets the internal Mesh*/
    InternalMesh& getMesh(MeshHandler handler);
    /** Resets the MeshHandlers*/
    void resetMesh(MeshHandler handler);
    /** Creates and updates the meshes to be drawn*/
    void updateMeshVertices(MeshHandler handler);

    // TEXTURE
    /** Creates a new mesh internally and returns a TextureHandler*/
    TextureHandler newTexture();
    /** Load the texture internally given a TextureHandler*/
    void loadTexture(TextureHandler handler,const char* src);
    /** Load the texture empty internally given a TextureHandler*/
    void loadEmpty(TextureHandler handler,int width, int height);
    /** Resizes texture internally given a TextureHandler*/
    void resize(TextureHandler handler,int width, int height);
    /** Load a cubemap texture internally given a TextureHandler*/
    void loadCubemap(TextureHandler handler,std::vector<std::string> path);
    /** Sets the format of the texture internally given a TextureHandler*/
    void setFormat(TextureHandler handler,Format tex_format);
    /** Sets the type of the texture internally given a TextureHandler*/
    void setType(TextureHandler handler, Type tex_type);
    /** Sets the filter of the texture internally given a TextureHandler*/
    void setFilter(TextureHandler handler, Filter tex_filter);
    /** Sets the wrap of the texture internally given a TextureHandler*/
    void setWrap(TextureHandler handler, Wrap tex_wrap);

    /** Sets current skybo*/
    void setSkybox(SkyboxMaterial& mat,Mesh& mesh);

    // Management
    /** Mesh management will create all the needed meshes of update them when needed*/
    void meshManagement();
    /** Texture management will create all the needed meshes of update them when needed*/
    void textureManagement();

    // Data
    InternalMesh meshes[20];
    InternalTexture textures[20];
    InternalSkybox skybox;
  };
}

#endif // _RESOURCE_MANAGER_H_
