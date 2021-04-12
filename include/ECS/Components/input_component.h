// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _INPUT_COMPONENT_H_
#define _INPUT_COMPONENT_H_ 1

#include <map>
#include <list>

namespace Aloe {

  struct InputComponent {
    std::map<char, std::list<void(*)()>> input_callbacks;
    std::list<void(*)(double pos_x, double pos_y)> mouse_callbacks;
  };
  
  void AddInputBinding(InputComponent& ic, const char key, void(*f)());
  void RemoveInputBinding(InputComponent& ic, const char key, void(*f)());

  void AddMouseInputBinding(InputComponent& ic, void(*f)(double pos_x, double pos_y));
  void RemoveMouseInputBinding(InputComponent& ic, void(*f)(double pos_x, double pos_y));
}

#endif