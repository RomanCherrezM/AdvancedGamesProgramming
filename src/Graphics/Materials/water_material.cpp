// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Graphics/Materials/water_material.h"
#include "glad/glad.h"

#include "ECS/Components/camera_component.h"
#include "Core/engine.h"

#include <stdio.h>

namespace Aloe {

  WaterMaterial::WaterMaterial() {
  }
  WaterMaterial::~WaterMaterial() {
  }

  void WaterMaterial::setupUniforms() {

    uniforms.insert(std::make_pair("height_map1", glGetUniformLocation(getHandler(), "height_map1")));
    uniforms.insert(std::make_pair("height_map2", glGetUniformLocation(getHandler(), "height_map2")));
    uniforms.insert(std::make_pair("normal_map1", glGetUniformLocation(getHandler(), "normal_map1")));
    uniforms.insert(std::make_pair("normal_map2", glGetUniformLocation(getHandler(), "normal_map2")));
    uniforms.insert(std::make_pair("time", glGetUniformLocation(getHandler(), "time")));
    uniforms.insert(std::make_pair("camera_pos", glGetUniformLocation(getHandler(), "camera_pos")));
    uniforms.insert(std::make_pair("model", glGetUniformLocation(getHandler(), "model")));
    uniforms.insert(std::make_pair("projection", glGetUniformLocation(getHandler(), "projection")));
    uniforms.insert(std::make_pair("view", glGetUniformLocation(getHandler(), "view")));

  }

  void WaterMaterial::use(TransformComponent* tr) {
    // Uniforms
    CameraComponent& c = Engine::get()->ecs->GetComponent<CameraComponent>(Engine::get()->main_camera);

    glUseProgram(getHandler());

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (getWireframe()) {
      glPolygonMode(GL_FRONT, GL_LINE);
      glPolygonMode(GL_BACK, GL_LINE);
    }
    else {
      glPolygonMode(GL_FRONT, GL_FILL);
      glPolygonMode(GL_BACK, GL_FILL);
    }

    //Textures
    glUniform1i(uniforms.at("height_map1"), 0);
    glUniform1i(uniforms.at("height_map2"), 1);
    glUniform1i(uniforms.at("normal_map1"), 2);
    glUniform1i(uniforms.at("normal_map2"), 3);

    glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, height_map1.getInternalHandler());

    glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
    glBindTexture(GL_TEXTURE_2D, height_map2.getInternalHandler());

    glActiveTexture(GL_TEXTURE0 + 2); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, normal_map1.getInternalHandler());

    glActiveTexture(GL_TEXTURE0 + 3); // Texture unit 1
    glBindTexture(GL_TEXTURE_2D, normal_map2.getInternalHandler());

    //Uniforms
    glUniform1f(uniforms.at("time"),GetTimeSinceStart());
    glUniform3fv(uniforms.at("camera_pos"),1,&c.position.x);
    glUniformMatrix4fv(uniforms.at("model"), 1, GL_FALSE, &tr->local[0][0]);
    glUniformMatrix4fv(uniforms.at("projection"), 1, GL_FALSE, &c.projection_matrix[0][0]);
    glUniformMatrix4fv(uniforms.at("view"), 1, GL_FALSE, &c.view_matrix[0][0]);
  }
}