// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Game/door.h"
#include "ECS/Components/transform_component.h"
#include "reactphysics3d/reactphysics3d.h"
#include "Core/engine.h"
#include "Game/game.h"

namespace Aloe {

  Door::Door() {
    tr = nullptr;
    open = false;
  }

  Door::~Door() {

  }

  void Door::update() {
    if (!open) {
      bool state = true;
      for (int i = 0; i < cubes.size(); ++i) {
        if (!game->moving_cubes[cubes[i]].in_place) {
          state = false;
        }
      }

      if (state && !open) {
        open = true;

        tr->position.y = -10.0f;

      }
    }
  }

  void Door::setCubes(std::vector<u32> c){
    cubes = c;
  }

}