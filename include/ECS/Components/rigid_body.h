#ifndef _RIGIDBODY_COMPONENT_H_
#define _RIGIDBODY_COMPONENT_H_ 1

#include "reactphysics3d/body/RigidBody.h"
#include "Core/platform_types.h"
#include "ECS/Components/transform_component.h"

const double PI = 3.14159265359;

namespace Aloe {

  enum BodyType {
    STATIC, KINEMATIC, DYNAMIC
  };

  class RigidBodyComponent {
  public:
    RigidBodyComponent();
    ~RigidBodyComponent();

    void create(TransformComponent &tr);

    void setTransform(TransformComponent& tr);
    void setType(BodyType type);
    void destroyBody();
    
    reactphysics3d::RigidBody *rigidBody;
  private:
    s32 refs;
  };

  glm::vec3 ToEulerAngles(reactphysics3d::Quaternion q);
}



#endif
