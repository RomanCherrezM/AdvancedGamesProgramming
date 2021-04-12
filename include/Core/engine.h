// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _ENGINE_H_
#define _ENGINE_H_ 1

#include "app.h"
#include "ECS/ecs_manager.h"
#include "ECS/Systems/input_system.h"
#include "ECS/Systems/render_system.h"
#include "ECS/Systems/transform_system.h"
#include "ECS/Systems/camera_system.h"
#include "ECS/Systems/physics_system.h"
#include "ECS/Components/camera_component.h"

#include "Graphics/resource_manager.h"

#include "Core/timer.h"
#include "Core/window.h"

#include "reactphysics3d/reactphysics3d.h"

namespace Aloe {

  ECSManager* GetGlobalECS();

  class Engine {

  public:
    static Engine* get();

    void Init();

    App app;

    // ECS 
    ECSManager* ecs;

    /** Input system*/
    std::shared_ptr<InputSystem> inputSystem;
    /** Render system*/
    std::shared_ptr<RenderSystem> renderSystem;
    /** Transform system*/
    std::shared_ptr<TransformSystem> transformSystem;
    /** Camera system*/
    std::shared_ptr<CameraSystem> cameraSystem;
    /** Physics system*/
    std::shared_ptr<PhysicsSystem> physicsSystem;

    /** Main camera*/
    Entity main_camera;

    /** Resouce Manager*/
    ResourceManager resource_manager;

    reactphysics3d::PhysicsWorld *physicsWorld;
    reactphysics3d::PhysicsCommon physicsCommon;

    //Time
    Timer timer;

    /** Engine default values setup*/
    void DefaultValuesSetup();
    /** Engine render setup*/
    void RenderSetup();
    /** Engine ECS setup, registers all the components, system and sets the signatures*/
    void ECSSetup(); 
    /** Physics setup*/
    void PhysicsSetup();

  private:
  };
}

#endif