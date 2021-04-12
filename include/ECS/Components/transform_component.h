#ifndef _TRANSFORM_COMPONENT_H_
#define _TRANSFORM_COMPONENT_H_ 1

#include "vec3.hpp"
#include "mat4x4.hpp"

namespace Aloe {

  struct TransformComponent {
    
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 local;

  };
}

#endif
