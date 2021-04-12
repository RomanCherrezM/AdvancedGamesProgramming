#include "ECS/Systems/physics_system.h"

#include "ECS/ecs_manager.h"
#include "Core/engine.h"

#include "ECS/Components/transform_component.h"
#include "ECS/Components/rigid_body.h"

namespace Aloe {

  void PhysicsSystem::Init() {
  
  }

  void PhysicsSystem::Update(float dt ) {
    
    Engine* e = Engine::get();
    ECSManager* ecs = e->ecs;

    //Set rigidBody Transform
    for each (auto entity in entities){

      TransformComponent &tr = ecs->GetComponent<TransformComponent>(entity);
      RigidBodyComponent &rb = ecs->GetComponent<RigidBodyComponent>(entity);

      rb.setTransform(tr);
    }

    e->physicsWorld->update(dt);

    //Fix Transform positions and rotations
    for each (auto entity in entities) {

      TransformComponent& tr = ecs->GetComponent<TransformComponent>(entity);
      RigidBodyComponent& rb = ecs->GetComponent<RigidBodyComponent>(entity);

      reactphysics3d::Vector3 p;
      p = rb.rigidBody->getTransform().getPosition();

      tr.position = glm::vec3(p.x,p.y,p.z);
      tr.rotation = ToEulerAngles(rb.rigidBody->getTransform().getOrientation());
    }
  }
}
