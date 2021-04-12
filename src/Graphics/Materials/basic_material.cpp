// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Graphics/Materials/basic_material.h"
#include "glad/glad.h"

#include "ECS/Components/camera_component.h"
#include "Core/engine.h"

#include <stdio.h>

namespace Aloe {

  BasicMaterial::BasicMaterial() {
  }
  BasicMaterial::~BasicMaterial() {
  }  

  void BasicMaterial::setupUniforms() {
    
    uniforms.insert(std::make_pair("base_texture", glGetUniformLocation(getHandler(), "base_texture")));
    uniforms.insert(std::make_pair("model", glGetUniformLocation(getHandler(), "model")));
    uniforms.insert(std::make_pair("projection", glGetUniformLocation(getHandler(), "projection")));
    uniforms.insert(std::make_pair("view", glGetUniformLocation(getHandler(), "view")));

  }

  void BasicMaterial::use(TransformComponent* tr) {
    // Uniforms
    CameraComponent& c = Engine::get()->ecs->GetComponent<CameraComponent>(Engine::get()->main_camera);

    glUseProgram(getHandler());

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    if (getWireframe()) {
      glPolygonMode(GL_FRONT, GL_LINE);
      glPolygonMode(GL_BACK, GL_LINE);
    }
    else {
      glPolygonMode(GL_FRONT, GL_FILL);
      glPolygonMode(GL_BACK, GL_FILL);
    }

    //Textures
    glUniform1i(uniforms.at("base_texture"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.getInternalHandler());

    //Uniforms
    glUniformMatrix4fv(uniforms.at("model"),1, GL_FALSE, &tr->local[0][0]);
    glUniformMatrix4fv(uniforms.at("projection"),1, GL_FALSE, &c.projection_matrix[0][0]);
    glUniformMatrix4fv(uniforms.at("view"),1, GL_FALSE, &c.view_matrix[0][0]);
  }
}