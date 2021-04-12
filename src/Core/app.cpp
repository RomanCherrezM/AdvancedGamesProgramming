#include "Core/app.h"

#include "Core/window.h"
#include "Core/engine.h"
#include "user_main.h"

#define UserStart Start
#define UserUpdate Update
#define UserClose Close

namespace Aloe {

  void App::EngineStart() {

    UserStart();
  }

  void App::EngineLoop() {

    Engine* e = Engine::get();

    double current_time = glfwGetTime();
    double time_step = 1.0 / 60.0;

    while (!window.shouldClose()) {
      glfwPollEvents();

      UserUpdate();

      window.update();

      // Resources
      if (window.isValid()) {
        e->resource_manager.meshManagement();
        e->resource_manager.textureManagement();
      }
      // Systems
      e->inputSystem->Update(1.0f);

      // Physics

      double accum_time = glfwGetTime() - current_time;

      while (accum_time >= time_step) {
        e->physicsSystem->Update(time_step);
        current_time += time_step;
        accum_time = glfwGetTime() - current_time;
      }

      e->transformSystem->Update(1.0f);
      e->cameraSystem->Update(1.0f);

      e->renderSystem->Update(1.0f);

      window.endFrame();
    }

    EngineClose();
  }

  void App::EngineClose() {
    UserClose();
  }
}

int main(int argc, char** argv){

  Aloe::App* app = &Aloe::Engine::get()->app;

  Aloe::Engine::get()->Init();

	app->EngineStart();
	app->EngineLoop();
	app->EngineClose();

	return 0;
}