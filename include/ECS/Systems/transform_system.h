#ifndef _TRANFORM_SYSTEM_H_
#define _TRANFORM_SYSTEM_H_ 1

#include "system.h"

namespace Aloe {
  class TransformSystem : public System{

  public:

    void Init();
    void Update(float dt);
  };

}

#endif