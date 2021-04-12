#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_ 1

#include "ECS/Systems/system.h"

namespace Aloe {

  class InputSystem : public System {

  public:
    void Init();

    void Update(float dt);
  };
}

#endif