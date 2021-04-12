// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "ECS/Components/rigid_body.h"

#include "Core/engine.h"

#include "glm.hpp"

namespace Aloe {

  RigidBodyComponent::RigidBodyComponent(){
  }

  RigidBodyComponent::~RigidBodyComponent() {
  }

  void RigidBodyComponent::create(TransformComponent& tr) {
    Engine* e = Engine::get();

    reactphysics3d::Transform p_tr;
    reactphysics3d::Quaternion quat;
    quat = reactphysics3d::Quaternion::fromEulerAngles(tr.rotation.x,tr.rotation.y,tr.rotation.z);

    p_tr.setPosition(reactphysics3d::Vector3(tr.position.x,tr.position.y,tr.position.z));
    p_tr.setOrientation(quat);   

    rigidBody = e->physicsWorld->createRigidBody(p_tr);
  }

  void RigidBodyComponent::destroyBody() {
    Engine* e = Engine::get();

    e->physicsWorld->destroyRigidBody(rigidBody);
  }

  void RigidBodyComponent::setTransform(TransformComponent& tr) {
    Engine* e = Engine::get();

    reactphysics3d::Transform p_tr;
    reactphysics3d::Quaternion quat;
    quat = reactphysics3d::Quaternion::fromEulerAngles(tr.rotation.x, tr.rotation.y, tr.rotation.z);

    p_tr.setPosition(reactphysics3d::Vector3(tr.position.x, tr.position.y, tr.position.z));
    p_tr.setOrientation(quat);

    rigidBody->setTransform(p_tr);
  }

  void RigidBodyComponent::setType(BodyType type){
    rigidBody->setType((reactphysics3d::BodyType)type);
  }

  glm::vec3 ToEulerAngles(reactphysics3d::Quaternion q) {
    glm::vec3 angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    angles.x = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2.0f * (q.w * q.y - q.z * q.x);
    if (std::abs(sinp) >= 1.0f)
      angles.y = std::copysign(PI / 2.0f, sinp); // use 90 degrees if out of range
    else
      angles.y = std::asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    angles.z = std::atan2(siny_cosp, cosy_cosp);

    return angles;
  }
}
