#ifndef _CAMERA_SYSTEM_H_
#define _CAMERA_SYSTEM_H_ 1

#include "system.h"

namespace Aloe {
  class CameraSystem : public System{

  public:

    void Init();
    void Update(float dt);
  };

}

#endif