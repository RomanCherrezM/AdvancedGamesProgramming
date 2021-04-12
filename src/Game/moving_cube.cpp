// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Game/moving_cube.h"
#include "ECS/Components/transform_component.h"
#include "reactphysics3d/reactphysics3d.h"
#include "Core/engine.h"
#include "Game/game.h"

namespace Aloe {

  MovingCube::MovingCube() {
    velocity = glm::vec3(0.0f);
    speed = 0.0f;
    tr = nullptr;
    in_place = false;
    type = Red_Cube;

    raycast_listener = new MyRaycastListener;
  }

  MovingCube::~MovingCube() {
    
  }

  void MovingCube::update() {
  
    tr->position += velocity * speed;
    
    //Trow raycast to direction to check a wall collision
    if (speed != 0.0f) {
      
      reactphysics3d::Vector3 start_point = reactphysics3d::Vector3(tr->position.x, tr->position.y, tr->position.z);
      reactphysics3d::Vector3 final_point = reactphysics3d::Vector3(tr->position.x, tr->position.y, tr->position.z)
        + reactphysics3d::Vector3(velocity.x, velocity.y, velocity.z) * 0.5f;

      reactphysics3d::Ray ray(start_point,final_point);

      raycast_listener->cube = this;
      Engine::get()->physicsWorld->raycast(ray, raycast_listener);
    }

  }

  void MovingCube::frontCollision() {
    printf("\nFront collision");
    // Snap position to grid
    printf("\n %f %f %f",tr->position.x,tr->position.y,tr->position.z);

    speed = 0.0f;
    velocity = glm::vec3(0.0f);
  }

  void MovingCube::playerCollision(TransformComponent* p_tr) {
  
    velocity = glm::normalize(tr->position - p_tr->position);
    velocity.y = 0.0f;
    if (abs(velocity.x) >= abs(velocity.z)) {
      velocity.z = 0.0f;
      if (velocity.x <= 0.0f) velocity.x = -1.0f; else velocity.x = 1.0f;
    }else {
      velocity.x = 0.0f;
      if (velocity.z <= 0.0f) velocity.z = -1.0f; else velocity.z = 1.0f;
    }

    speed = 0.1f;
  }

}