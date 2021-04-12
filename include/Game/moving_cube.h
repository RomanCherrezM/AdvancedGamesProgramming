// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _MOVING_CUBE_
#define _MOVING_CUBE_ 1

#include "ECS/ecs_constants.h"
#include "vec3.hpp"
#include "Game/detector_cube.h"

namespace Aloe {

  class TransformComponent;
  class RenderComponent;
  class MyRaycastListener;

  class MovingCube {
  public:
    MovingCube();
    ~MovingCube();

    void update();
    void playerCollision(TransformComponent* p_tr);
    void frontCollision();

    MyRaycastListener *raycast_listener;

    Entity entity;
    TransformComponent* tr;
    RenderComponent* rd;

    glm::vec3 velocity;
    float speed;
    bool in_place;

    CubeType type;
  };
}

#endif