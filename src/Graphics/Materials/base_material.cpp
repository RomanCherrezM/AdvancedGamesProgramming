// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Graphics/Materials/base_material.h"
#include "Graphics/resource_manager.h"

#include "Core/engine.h"

#include "glad/glad.h"

#include <stdio.h>

namespace Aloe {

  struct BaseMaterial::Data{
    //OPENGL PROGRAM
    MaterialHandler program = -1;

    DrawMode draw_mode = Triangles;

    Shader v_shader;
    Shader f_shader;
    Shader g_shader;

    bool wireframe = false;
  };

  BaseMaterial::BaseMaterial() {
    data = new Data;
    data->program = -1;
  }

  BaseMaterial::~BaseMaterial() {}

  void BaseMaterial::create(Shader vertex, Shader fragment) {
    ResourceManager* r = &Engine::get()->resource_manager;

    data->v_shader = vertex;
    data->f_shader = fragment;

    data->program = glCreateProgram();

    glAttachShader(data->program, vertex.get());
    glAttachShader(data->program, fragment.get());

    glLinkProgram(data->program);

    if (glGetError() != GL_NO_ERROR) {
      printf("\n [ERROR] [MATERIAL] : GL ERROR in create1 of BaseMaterial");
    }
  }

  void BaseMaterial::create(Shader vertex, Shader fragment, Shader geometry) {
    ResourceManager* r = &Engine::get()->resource_manager;

    data->v_shader = vertex;
    data->f_shader = fragment;
    data->g_shader = geometry;

    data->program = glCreateProgram();

    glAttachShader(data->program, vertex.get());
    glAttachShader(data->program, geometry.get());
    glAttachShader(data->program, fragment.get());

    glLinkProgram(data->program);

    if (glGetError() != GL_NO_ERROR) {
      printf("\n [ERROR] [MATERIAL] : GL ERROR in create2 of BaseMaterial");
    }
  }

  void BaseMaterial::recompileShaders() {
  
    if (data->v_shader.valid())glDetachShader(data->program,data->v_shader.get());
    if (data->f_shader.valid())glDetachShader(data->program,data->f_shader.get());
    if (data->g_shader.valid())glDetachShader(data->program,data->g_shader.get());

    data->v_shader.recompile();
    data->f_shader.recompile();
    data->g_shader.recompile();
  
    if (data->v_shader.valid())glAttachShader(data->program, data->v_shader.get());
    if (data->f_shader.valid())glAttachShader(data->program, data->f_shader.get());
    if (data->g_shader.valid())glAttachShader(data->program, data->g_shader.get());

    glLinkProgram(data->program);

    if (glGetError() != GL_NO_ERROR) {
      printf("\n [ERROR] [MATERIAL] : GL ERROR in recompileShaders of BaseMaterial");
    }
  }

  void BaseMaterial::setupUniforms() {}
  void BaseMaterial::use(TransformComponent* tr) {}

  void BaseMaterial::setDrawMode(DrawMode drawmode){
    data->draw_mode = drawmode;
  }

  DrawMode BaseMaterial::getDrawMode(){
    return data->draw_mode;
  }

  void BaseMaterial::setWireframe(bool w){
    data->wireframe = w;
  }

  bool BaseMaterial::getWireframe(){
    return data->wireframe;
  }

  MaterialHandler BaseMaterial::getHandler(){
    return data->program;
  }


}