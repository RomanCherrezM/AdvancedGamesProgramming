#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_ 1

#include "ECS/Systems/system.h"

namespace Aloe {

  class RenderSystem : public System {

  public:
    void Init();

    void Update(float dt);

  };
}

#endif