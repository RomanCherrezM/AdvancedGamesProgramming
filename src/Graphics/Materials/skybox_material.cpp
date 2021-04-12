// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Graphics/Materials/skybox_material.h"
#include "glad/glad.h"

#include "ECS/Components/camera_component.h"
#include "Core/engine.h"

#include <stdio.h>

namespace Aloe {

  SkyboxMaterial::SkyboxMaterial() {
  }
  SkyboxMaterial::~SkyboxMaterial() {
  }  

  void SkyboxMaterial::setupUniforms() {

    uniforms.insert(std::make_pair("skybox", glGetUniformLocation(getHandler(), "skybox")));
    uniforms.insert(std::make_pair("projection", glGetUniformLocation(getHandler(), "projection")));
    uniforms.insert(std::make_pair("view", glGetUniformLocation(getHandler(), "view")));

    if (glGetError() != GL_NO_ERROR) {
      printf("\n [ERROR] [SKYBOX MATERIAL] : GL ERROR setupUniforms");
    }
  }

  void SkyboxMaterial::use(TransformComponent* tr) {
    // Uniforms
    CameraComponent& c = Engine::get()->ecs->GetComponent<CameraComponent>(Engine::get()->main_camera);

    glUseProgram(getHandler());

    glDepthMask(GL_FALSE);
    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    if (getWireframe()) {
      glPolygonMode(GL_FRONT, GL_LINE);
      glPolygonMode(GL_BACK, GL_LINE);
    }else {
      glPolygonMode(GL_FRONT, GL_FILL);
      glPolygonMode(GL_BACK, GL_FILL);
    }

    //Textures
    glUniform1i(uniforms.at("skybox"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.getInternalHandler());

    glm::mat4x4 m = c.view_matrix;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;

    //Uniforms
    glUniformMatrix4fv(uniforms.at("projection"),1, GL_FALSE, &c.projection_matrix[0][0]);
    glUniformMatrix4fv(uniforms.at("view"),1, GL_FALSE, &m[0][0]);


    if (glGetError() != GL_NO_ERROR) {
      printf("\n [ERROR] [SKYBOX MATERIAL] : GL ERROR use");
    }
  }
}