// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _DOOR_
#define _DOOR_ 1

#include "ECS/ecs_constants.h"
#include "vec3.hpp"
#include <vector>

namespace Aloe {

  class TransformComponent;
  class RenderComponent;
  class MyRaycastListener;
  class Game;
  class MovingCube;

  class Door {
  public:
    Door();
    ~Door();

    /** Updates the logic of the cube*/
    void update();
    /** Sets the indices that references the detector cubes*/
    void setCubes(std::vector<u32> c);

    /** Reference to the game*/
    Game* game;

    /** Cube's TransformComponent*/
    TransformComponent* tr;

    /** Detector cubes*/
    std::vector<u32> cubes;
    /** Door state*/
    bool open;
  };
}

#endif