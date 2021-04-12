// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#define STB_IMAGE_IMPLEMENTATION

#include "Graphics/resource_manager.h"

#include "glad/glad.h"
#include "Core/platform_types.h"
#include "stb_image.h"
#include <vector>

#define MAX_MESHES 20
#define MAX_TEXTURES 20

namespace Aloe {

  ResourceManager::ResourceManager() {
  }

  ResourceManager::~ResourceManager() {
  }

  // MESH
  MeshHandler ResourceManager::newMesh(){
    for (int i = 0; i < MAX_MESHES;++i) {
      if (meshes[i].state == AVAILABLE) {
        meshes[i].state = TO_BE_CREATED;
        return i;
      }
    }   
    printf("\n[ERROR] [RESOURCE MANAGER]: Not Enough Buffers");
    return -1;
  }
  void ResourceManager::setMeshPositions(MeshHandler handler, const float* v, u32 size){
    if (meshes[handler].state == TO_BE_CREATED) {
      meshes[handler].positions.alloc(size);
      meshes[handler].positions.memCopy(0, &v[0], size);
    }else {
      meshes[handler].state = TO_BE_UPDATED;
      meshes[handler].positions.memCopy(0, &v[0], size);
    }
  }
  void ResourceManager::setMeshNormals(MeshHandler handler, const float* v, u32 size) {
    if (meshes[handler].state == TO_BE_CREATED) {
      meshes[handler].normals.alloc(size);
      meshes[handler].normals.memCopy(0, &v[0], size);
    }else {
      meshes[handler].state = TO_BE_UPDATED;
      meshes[handler].normals.memCopy(0, &v[0], size);
    }
  }
  void ResourceManager::setMeshUvs(MeshHandler handler, const float* v, u32 size) {
    if (meshes[handler].state == TO_BE_CREATED) {
      meshes[handler].uvs.alloc(size);
      meshes[handler].uvs.memCopy(0, &v[0], size);
    }else {
      meshes[handler].state = TO_BE_UPDATED;
      meshes[handler].uvs.memCopy(0, &v[0], size);
    }
  }
  void ResourceManager::setMeshIndices(MeshHandler handler, const u32* i, u32 size){
    if (meshes[handler].state == TO_BE_CREATED) {
      meshes[handler].indices.alloc(size);
      meshes[handler].indices.memCopy(0, &i[0], size);
    }else {
      meshes[handler].state = TO_BE_UPDATED;
      meshes[handler].indices.memCopy(0, &i[0], size);
    }
  }
  InternalMesh& ResourceManager::getMesh(MeshHandler handler){
    return meshes[handler];
  }
  void ResourceManager::resetMesh(MeshHandler handler) {
    meshes[handler].state = AVAILABLE;
    meshes[handler].positions.release();
    meshes[handler].normals.release();
    meshes[handler].uvs.release();
    meshes[handler].indices.release();
  }
  void ResourceManager::updateMeshVertices(MeshHandler handler) {
    int position_size = meshes[handler].positions.size();
    int normal_size = meshes[handler].normals.size();
    int uv_size = meshes[handler].uvs.size();

    int total_size = position_size + normal_size + uv_size;

    meshes[handler].vertices.release();
    meshes[handler].vertices.alloc(total_size);

    if (meshes[handler].positions.size() > 0)
      meshes[handler].vertices.memCopy(0, &meshes[handler].positions[0],position_size);
    if (meshes[handler].normals.size() > 0)
      meshes[handler].vertices.memCopy(position_size, &meshes[handler].normals[0],normal_size);
    if (meshes[handler].uvs.size() > 0)
      meshes[handler].vertices.memCopy(position_size + normal_size, &meshes[handler].uvs[0],uv_size);
  
  }


  TextureHandler ResourceManager::newTexture(){
    for (int i = 0; i < MAX_TEXTURES; ++i) {
      if (textures[i].state == AVAILABLE) {
        textures[i].state = TO_BE_CREATED;
        return i;
      }
    }
    printf("\n[ERROR] [RESOURCE MANAGER]: Not Enough Textures");
    return -1;
  }
  void ResourceManager::loadTexture(TextureHandler handler, const char* src){

    int width, height, num_colors;

    unsigned char* data = stbi_load(src,&width,&height,&num_colors,0);

    textures[handler].width = width;
    textures[handler].height = height;
    textures[handler].num_colors = num_colors;
    //Choosing a default "format" based in the ChannelColors of the image.
    switch (num_colors) {
    case 1: textures[handler].format = F_R; break;
    case 2: textures[handler].format = F_RG; break;
    case 3: textures[handler].format = F_RGB; break;
    case 4: textures[handler].format = F_RGBA; break;
    default: textures[handler].format = F_None; break;
    }
    textures[handler].type = Type::T_2D;
    textures[handler].filter = Filter::F_LINEAR;
    textures[handler].wrap = Wrap::W_REPEAT;

    textures[handler].data.alloc(width * height * num_colors);
    textures[handler].data.memCopy(0, data, textures[handler].data.size());

    textures[handler].state = TO_BE_CREATED;
  }
  void ResourceManager::loadEmpty(TextureHandler handler, int width, int height){
    printf("\n Not implemented");
  }
  void ResourceManager::resize(TextureHandler handler, int width, int height){
    printf("\n Not implemented");
  }
  void ResourceManager::loadCubemap(TextureHandler handler, std::vector<std::string> path){


    int width = 0, height = 0, num_colors = 0;
    int total_size = 0;
    std::vector<unsigned char*> data;
    for (GLuint i = 0; i < path.size(); i++) {
      data.push_back(stbi_load(path[i].c_str(), &width, &height, &num_colors, 0));
      total_size += width * height * num_colors;
    }

    textures[handler].data.alloc(total_size);

    for (GLuint i = 0; i < path.size(); i++) {
      textures[handler].data.memCopy(i * width * height * num_colors, data[i], width * height * num_colors);
    }

    textures[handler].width = width;
    textures[handler].height = height;
    textures[handler].num_colors = num_colors;
    switch (textures[handler].num_colors) {
    case 1:textures[handler].format = F_R; break;
    case 2:textures[handler].format = F_RG; break;
    case 3:textures[handler].format = F_RGB; break;
    case 4:textures[handler].format = F_RGBA; break;
    default:textures[handler].format = F_None; break;
    }

    textures[handler].type = T_CUBEMAP;
    textures[handler].filter = F_NEAREST;
    textures[handler].wrap = W_REPEAT;

    textures[handler].state = TO_BE_CREATED;
  }
  void ResourceManager::setFormat(TextureHandler handler, Format tex_format){
    printf("\n Not implemented");
  }
  void ResourceManager::setType(TextureHandler handler, Type tex_type){
    printf("\n Not implemented");
  }
  void ResourceManager::setFilter(TextureHandler handler, Filter tex_filter){
    printf("\n Not implemented");
  }
  void ResourceManager::setWrap(TextureHandler handler, Wrap tex_wrap){
    printf("\n Not implemented");
  }

  void ResourceManager::setSkybox(SkyboxMaterial& mat, Mesh& mesh){
    skybox.material = mat;
    skybox.mesh = mesh;
  }

  // Management
  void ResourceManager::meshManagement(){
    for (int i = 0; i < MAX_MESHES; ++i) {

      switch (meshes[i].state) {
      case TO_BE_CREATED:
        glGenBuffers(1, &meshes[i].vertex_gl_id);
        glGenBuffers(1, &meshes[i].index_gl_id);

      case TO_BE_UPDATED:
        //Vertex
        glBindBuffer(GL_ARRAY_BUFFER, meshes[i].vertex_gl_id);
        int size = meshes[i].positions.sizeInBytes() + meshes[i].normals.sizeInBytes() + meshes[i].uvs.sizeInBytes();
        updateMeshVertices(i);
        glBufferData(GL_ARRAY_BUFFER, size, &meshes[i].vertices[0], GL_STATIC_DRAW);

        //Index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].index_gl_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[i].indices.sizeInBytes(), &meshes[i].indices[0], GL_STATIC_DRAW);
        meshes[i].state = CREATED;

        break;
      }
    }

    if (glGetError() != GL_NO_ERROR) {
      printf("\n [ERROR] [MESH] : GL ERROR");
    }
  }

  static GLenum Translate(Format &f) {
    switch (f) {
      case Format::F_None:return GL_NONE;break;
      case Format::F_R:return GL_RED;break;
      case Format::F_RG:return GL_RG;break;
      case Format::F_RGB:return GL_RGB;break;
      case Format::F_BGRA:return GL_BGRA;break;
      case Format::F_RGBA:return GL_RGBA;break;
      case Format::F_DEPTH:return GL_DEPTH;break;
      case Format::F_DEPTH_COMPONENT:return GL_DEPTH_COMPONENT;break;
      case Format::F_DEPTH32:return GL_DEPTH_COMPONENT32;break;
    }
    return GL_NONE;
  }
  static GLenum Translate(Type& t) {
    switch (t) {
      case Type::T_1D:       return GL_TEXTURE_1D; break;
      case Type::T_2D:       return GL_TEXTURE_2D; break;
      case Type::T_3D:       return GL_TEXTURE_3D; break;
      case Type::T_CUBEMAP:  return GL_TEXTURE_CUBE_MAP;break;
    }
    return GL_NONE;
  }
  static GLenum Translate(Filter& f) {
    switch (f) {
      case Filter::F_NEAREST:return GL_NEAREST;break;
      case Filter::F_LINEAR:return GL_LINEAR;break;
      case Filter::F_NEAREST_MIPMAP_NEAREST:return GL_NEAREST_MIPMAP_NEAREST;break;
      case Filter::F_LINEAR_MIPMAP_NEAREST:return GL_LINEAR_MIPMAP_NEAREST;break;
      case Filter::F_NEAREST_MIPMAP_LINEAR:return GL_NEAREST_MIPMAP_LINEAR;break;
      case Filter::F_LINEAR_MIPMAP_LINEAR:return GL_LINEAR_MIPMAP_LINEAR;break;
    }
    return GL_NEAREST;
  }
  static GLenum Translate(Wrap& w) {
    switch (w) {
      case Wrap::W_REPEAT:return GL_REPEAT;break;
      case Wrap::W_MIRRORED_REPEAT:return GL_MIRRORED_REPEAT;break;
      case Wrap::W_CLAMP_TO_EDGE:return GL_CLAMP_TO_EDGE;break;
    }
    return GL_REPEAT;
  }

  void ResourceManager::textureManagement() {
    for (int i = 0; i < MAX_TEXTURES; ++i) {
      switch (textures[i].state) {
      case TO_BE_CREATED:
        glGenTextures(1, &textures[i].texture);
      case TO_BE_UPDATED:
        glBindTexture(Translate(textures[i].type), textures[i].texture);

        glTexParameteri(Translate(textures[i].type),GL_TEXTURE_WRAP_S,Translate(textures[i].wrap));
        glTexParameteri(Translate(textures[i].type),GL_TEXTURE_WRAP_T,Translate(textures[i].wrap));
        glTexParameteri(Translate(textures[i].type), GL_TEXTURE_MIN_FILTER,Translate(textures[i].filter));
        glTexParameteri(Translate(textures[i].type),GL_TEXTURE_MAG_FILTER, Translate(textures[i].filter));

        //No pow2 texttures
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        if (textures[i].type != Type::T_CUBEMAP){
          glTexImage2D(Translate(textures[i].type), 0, Translate(textures[i].format),
            textures[i].width, textures[i].height, 0, Translate(textures[i].format),
            GL_UNSIGNED_BYTE, textures[i].data.size() == 0 ? nullptr : &textures[i].data[0]);
        }else {
          int w = textures[i].width;
          int h = textures[i].height;
          int n = textures[i].num_colors;
          for (int j = 0; j < 6; j++) {
            glTexImage2D(
              GL_TEXTURE_CUBE_MAP_POSITIVE_X + j,
              0, Translate(textures[i].format), textures[i].width, textures[i].height,
              0, Translate(textures[i].format), GL_UNSIGNED_BYTE, &textures[i].data[w * h * n * j]);
          }
        }
        glGenerateMipmap(Translate(textures[i].type));
        textures[i].state = CREATED;
        break;
      }
    }
    if (glGetError() != GL_NO_ERROR) {
      printf("\n [ERROR] [TEXTURE] : GL ERROR");
    }
  }
}