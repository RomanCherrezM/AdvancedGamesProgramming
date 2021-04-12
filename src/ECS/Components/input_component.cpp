#include "ECS/Components/input_component.h"

namespace Aloe {

  void AddInputBinding(InputComponent& ic, const char key, void(*f)()) {
    ic.input_callbacks[key].push_back(f);
  }
  void RemoveInputBinding(InputComponent& ic, const char key, void(*f)()) {
    ic.input_callbacks[key].remove(f);
  }

  void AddMouseInputBinding(InputComponent& ic, void(*f)(double pos_x, double pos_y)){
    ic.mouse_callbacks.push_back(f);
  }

  void RemoveMouseInputBinding(InputComponent& ic, void(*f)(double pos_x, double pos_y)) {
    ic.mouse_callbacks.remove(f);
  }

}