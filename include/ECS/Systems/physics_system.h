#ifndef _PHYSICS_SYSTEM_H_
#define _PHYSICS_SYSTEM_H_ 1

#include "system.h"

namespace Aloe {
  class PhysicsSystem : public System{

  public:

    void Init();
    void Update(float dt);
  };

}

#endif