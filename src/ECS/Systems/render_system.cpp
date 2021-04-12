#include "ECS/Systems/render_system.h"

#define PX_MEM_IMPLEMENTATION 1

#include "Core/engine.h"
#include "Graphics/mesh.h"
#include "Graphics/Materials/base_material.h"
#include "Graphics/Materials/basic_material.h"
#include "ECS/ecs_manager.h"
#include "ECS/Components/transform_component.h"
#include "ECS/Components/camera_component.h"
#include "ECS/Components/render_component.h"

#include "mat4x4.hpp"
#include "glad/glad.h"

namespace Aloe {

  void RenderSystem::Init() {
  
  }

  GLenum Translate(DrawMode d) {
    switch (d) {
    case Triangles:return GL_TRIANGLES;
    case Points:return GL_POINTS;
    }
  }

  void RenderSystem::Update(float dt) {
    ECSManager* ecs = Engine::get()->ecs;
    ResourceManager* r = &Engine::get()->resource_manager;

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Render skybox
    InternalSkybox* skybox = &r->skybox;
    if (r->skybox.material.getHandler() != -1) {
    
      InternalMesh* mesh = &r->getMesh(skybox->mesh.getHandler());

      skybox->material.use();

      glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_gl_id);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_gl_id);

      if (mesh->positions.size() != 0) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
      }

      glDrawElements(Translate(r->skybox.material.getDrawMode()), mesh->indices.size(), GL_UNSIGNED_INT, (void*)0);
    }

    for each (auto entity in entities){

      // get mesh component
      RenderComponent render = ecs->GetComponent<RenderComponent>(entity);
      TransformComponent tr = ecs->GetComponent<TransformComponent>(entity);

      //Material
      render.material->use(&tr); 

      InternalMesh* inner_mesh = &r->meshes[render.mesh->getHandler()];

      glBindBuffer(GL_ARRAY_BUFFER, inner_mesh->vertex_gl_id);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inner_mesh->index_gl_id);

      if (inner_mesh->positions.size() != 0) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);
      }

      if (inner_mesh->normals.size() != 0) {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(inner_mesh->positions.sizeInBytes()));
        glEnableVertexAttribArray(1);
      }

      if (inner_mesh->uvs.size() != 0) {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(inner_mesh->positions.sizeInBytes() + inner_mesh->normals.sizeInBytes()));
        glEnableVertexAttribArray(2);
      }

      glDrawElements(Translate(render.material->getDrawMode()), inner_mesh->indices.size(), GL_UNSIGNED_INT, (void*)0);

      if (glGetError() != GL_NO_ERROR) {
        printf("\n [ERROR] [RENDER_SYSTEM] : GL ERROR");
      }
    }
  }

}