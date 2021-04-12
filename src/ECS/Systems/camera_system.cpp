#include "ECS/Systems/camera_system.h"

#include "ECS/Components/camera_component.h"
#include "ECS/ecs_manager.h"
#include "Core/engine.h"

#include "gtx/transform.hpp"

namespace Aloe {

  void CameraSystem::Init() {
  
  }

  void CameraSystem::Update(float dt ) {
    
    ECSManager* ecs = Engine::get()->ecs;

    for each (auto entity in entities){

      CameraComponent &c = ecs->GetComponent<CameraComponent>(entity);

      c.view_matrix = glm::lookAt(c.position, c.position + c.front, c.up);

      c.projection_matrix = glm::perspective(glm::radians(c.fov),
        c.width / c.height,c.near_plane, c.far_plane);

      c.orthographic_matrix = glm::ortho(c.left, c.right, c.bottom, c.top, c.near_plane, c.far_plane);
      
    }
  }
}
