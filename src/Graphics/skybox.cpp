// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Graphics/skybox.h"
#include "Core/engine.h"

#include "glad/glad.h"

namespace Aloe {

  struct Skybox::Data {
    SkyboxMaterial skybox_material;
    Mesh skybox_mesh;
  };

  Skybox::Skybox() {
    data = new Data;
  }

  Skybox::~Skybox() {
  }

  Skybox& Skybox::create(std::vector<std::string> path) {

    // Change to shader in code
    Shader vertex_shader;
    vertex_shader.create("../../src/Shaders/shader_skybox.vert", ShaderVertex);
    Shader fragment_shader;
    fragment_shader.create("../../src/Shaders/shader_skybox.frag", ShaderFragment);

    Texture texture;
    texture.create().loadCubemap(path);

    if (glGetError() != GL_NO_ERROR) {
      printf("\n [ERROR] [MATERIAL] : GL ERROR 0");
    }

    data->skybox_material.create(vertex_shader, fragment_shader);

    if (glGetError() != GL_NO_ERROR) {
      printf("\n [ERROR] [SKYBOX MATERIAL] : GL ERROR 1");
    }

    data->skybox_material.setupUniforms();

    data->skybox_material.texture = texture;

    float cube_positions[] = {
      //TOP FACE
      -1.0f,  1.0f,  1.0f,			//0
      1.0f, 1.0f,  1.0f,				//1
      1.0f, 1.0f,  -1.0f,				//2
      -1.0f, 1.0f,  -1.0f,			//3

      //BACK FACE
      -1.0f, 1.0f,  -1.0f,			//4
      1.0f, 1.0f,  -1.0f,				//5
      1.0f, -1.0f,  -1.0f,			//6
      -1.0f,  -1.0f,  -1.0f,		//7

      //RIGHT FACE
      1.0f,  1.0f,  -1.0f,			//8
      1.0f,  1.0f,  1.0f,				//9
      1.0f,  -1.0f,  1.0f,			//10
      1.0f, -1.0f,  -1.0f,			//11

      //LEFT FACE
      -1.0f, 1.0f, 1.0f,				//12
      -1.0f, 1.0f, -1.0f,				//13
      -1.0f, -1.0f, -1.0f,			//14
      -1.0f, -1.0f, 1.0f,				//15

      //FRONT FACE
      1.0f, 1.0f, 1.0f,					//16
      -1.0f, 1.0f, 1.0f,				//17
      -1.0f,  -1.0f, 1.0f,			//18
      1.0f,  -1.0f, 1.0f,				//19

      //BOTTOM FACE
      1.0f,  -1.0f, -1.0f,			//20
      -1.0f,  -1.0f, -1.0f,			//21
      -1.0f,  -1.0f, 1.0f,			//22
      1.0f,  -1.0f, 1.0f 			//23

    };
    unsigned int cube_indices[] = {
      0,1,2,0,2,3,
      4,5,6,4,6,7,
      8,9,10,8,10,11,
      12,13,14,12,14,15,
      16,17,18,16,18,19,
      20,22,21,20,23,22 };
    data->skybox_mesh.setPositions(cube_positions, sizeof(cube_positions) / sizeof(float));
    data->skybox_mesh.setIndices(cube_indices, sizeof(cube_indices) / sizeof(u32));

    return *this;
  }

  Texture& Skybox::getTexture(){
    return data->skybox_material.texture;
  }

  void Skybox::setAsCurrent() {  
    Engine* e = Engine::get();
    e->resource_manager.setSkybox(data->skybox_material,data->skybox_mesh);
  }
}
