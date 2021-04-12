// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Core/engine.h"

#include "ECS/Components/input_component.h"
#include "ECS/Components/transform_component.h"
#include "ECS/Components/render_component.h"
#include "ECS/Components/rigid_body.h"

#include "glad/glad.h"

namespace Aloe {

  Engine* Engine::get() {
    static Engine* instance = new Engine;

    return instance;
  }

  void Engine::DefaultValuesSetup() {
    main_camera = -1;

    timer.StartTimer();
  }

  void Engine::Init(){
    DefaultValuesSetup();
    ECSSetup();
    RenderSetup();
    PhysicsSetup();
  }

  void Engine::ECSSetup() {
    //Create ECS
    ecs = new ECSManager();
    ecs->Init();

    // Register components used
    ecs->RegisterComponent<TransformComponent>();
    ecs->RegisterComponent<InputComponent>();
    ecs->RegisterComponent<RenderComponent>();
    ecs->RegisterComponent<CameraComponent>();
    ecs->RegisterComponent<RigidBodyComponent>();

    // Register systems used
    inputSystem = ecs->RegisterSystem<InputSystem>();
    renderSystem = ecs->RegisterSystem<RenderSystem>();
    transformSystem = ecs->RegisterSystem<TransformSystem>();
    cameraSystem = ecs->RegisterSystem<CameraSystem>();
    physicsSystem = ecs->RegisterSystem<PhysicsSystem>();

    // Input Signature
    {
      Signature signature;
      signature.set(ecs->GetComponentType<InputComponent>());
      ecs->SetSystemSignature<InputSystem>(signature);
    }
    inputSystem->Init();

    // Physics Signature
    {
      Signature signature;
      signature.set(ecs->GetComponentType<TransformComponent>());
      signature.set(ecs->GetComponentType<RigidBodyComponent>());
      ecs->SetSystemSignature<PhysicsSystem>(signature);
    }
    physicsSystem->Init();

    // Render signature
    {
      Signature signature;
      signature.set(ecs->GetComponentType<RenderComponent>());
      signature.set(ecs->GetComponentType<TransformComponent>());
      ecs->SetSystemSignature<RenderSystem>(signature);
    }
    renderSystem->Init();

    // Transform Signature
    {
      Signature signature;
      signature.set(ecs->GetComponentType<TransformComponent>());
      ecs->SetSystemSignature<TransformSystem>(signature);
    }
    transformSystem->Init();

    // Camera Signature
    {
      Signature signature;
      signature.set(ecs->GetComponentType<CameraComponent>());
      ecs->SetSystemSignature<CameraSystem>(signature);
    }
    cameraSystem->Init();

  }

  void Engine::RenderSetup() {

  }

  void Engine::PhysicsSetup() {

    reactphysics3d::PhysicsWorld::WorldSettings settings;

    settings.gravity = reactphysics3d::Vector3(0,-9.81,0);
    settings.defaultVelocitySolverNbIterations = 20;
    settings.isSleepingEnabled = false;

    physicsWorld = physicsCommon.createPhysicsWorld(settings);
  
  
  }

  ECSManager* GetGlobalECS(){
    return Engine::get()->ecs;
  }

}