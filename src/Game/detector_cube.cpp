// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Game/detector_cube.h"
#include "ECS/Components/transform_component.h"
#include "reactphysics3d/reactphysics3d.h"
#include "Core/engine.h"
#include "Game/game.h"

namespace Aloe {

  DetectorCube::DetectorCube() {
    tr = nullptr;
    type = Red_Cube;
  }

  DetectorCube::~DetectorCube() {

  }

  void DetectorCube::update() {
  
    //Throw ray upwards
    reactphysics3d::Vector3 start_point = reactphysics3d::Vector3(tr->position.x, tr->position.y, tr->position.z);
    reactphysics3d::Vector3 final_point = reactphysics3d::Vector3(tr->position.x, tr->position.y + 10.0f, tr->position.z);

    reactphysics3d::Ray ray(start_point, final_point);
    for (int i = 0; i < game->moving_cubes.size();++i) {
      
      RigidBodyComponent &rb = game->ecs->GetComponent<RigidBodyComponent>(game->moving_cubes[i].entity);

      reactphysics3d::RaycastInfo info;
      if (rb.rigidBody->getCollider(0)->raycast(ray, info)) {
        if (type == game->moving_cubes[i].type && game->moving_cubes[i].speed == 0.0f && !game->moving_cubes[i].in_place) {
          game->moving_cubes[i].in_place = true;
          printf("\nIN_PLACE");
          //Play noise
        }
      }
    }
  }
}