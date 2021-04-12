// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _DETECTOR_CUBE_
#define _DETECTOR_CUBE_ 1

#include "ECS/ecs_constants.h"
#include "vec3.hpp"

namespace Aloe {

  class TransformComponent;
  class RenderComponent;
  class MyRaycastListener;
  class Game;

  enum CubeType {

    Red_Cube,
    Blue_Cube,
    Green_Cube,

  };

  class DetectorCube {
  public:
    DetectorCube();
    ~DetectorCube();

    /** Updates the logic of the cube*/
    void update();

    /** Reference to the game*/
    Game* game;

    /** Cube's entity*/
    Entity entity;
    /** Cube's TransformComponent*/
    TransformComponent* tr;
    /** Cube's RenderComponent*/
    RenderComponent* rd;

    /** Color of the block */
    CubeType type;
  };
}

#endif