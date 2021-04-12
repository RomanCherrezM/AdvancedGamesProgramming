#include "ECS/Systems/input_system.h"
#include "ECS/ecs_manager.h"
#include "ECS/Components/input_component.h"
#include "Core/engine.h"

#include "GLFW/glfw3.h"

namespace Aloe {

  void InputSystem::Init() {
  }

  void InputSystem::Update(float dt) {
  
    ECSManager* ecs = GetGlobalECS();

    for (auto const& entity : entities) {
      auto& input = ecs->GetComponent<InputComponent>(entity);

      GLFWwindow* w = Engine::get()->app.window.getNativeWindow();

      for each (auto pair in input.input_callbacks){
        int state = glfwGetKey(w,(int) pair.first);
        if (state == GLFW_PRESS) {
          for each (auto function in pair.second){
            function();
          }
        }
      }
      for each (auto function in input.mouse_callbacks) {
        double x, y;
        glfwGetCursorPos(w,&x,&y);
        function(x,y);
      }
    }


  }
}
