#include "ECS/Systems/transform_system.h"

#include "ECS/Components/transform_component.h"
#include "ECS/ecs_manager.h"
#include "Core/engine.h"

#include "gtx/transform.hpp"

namespace Aloe {

  void TransformSystem::Init() {
  
  }

  void TransformSystem::Update(float dt ) {
    
    ECSManager* ecs = Engine::get()->ecs;

    for each (auto entity in entities){

      TransformComponent &tr = ecs->GetComponent<TransformComponent>(entity);

      glm::mat4 tMat = glm::translate(tr.position);
      glm::mat4 rxMat = glm::rotate(tr.rotation.x, glm::vec3(1, 0, 0));
      glm::mat4 ryMat = glm::rotate(tr.rotation.y, glm::vec3(0, 1, 0));
      glm::mat4 rzMat = glm::rotate(tr.rotation.z, glm::vec3(0, 0, 1));
      glm::mat4 sMat = glm::scale(tr.scale);

      tr.local = tMat * rzMat * ryMat * rxMat * sMat;
    }
  }
}
