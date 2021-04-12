#include "ECS/Components/camera_component.h"

#include "gtc/matrix_transform.hpp"

#include "Core/engine.h"

namespace Aloe {

  CameraComponent::CameraComponent() {
  }

  CameraComponent& CameraComponent::setViewMatrix(glm::vec3 position,
    glm::vec3 front, glm::vec3 up) {

    this->position = position;
    this->up = up;
    this->front = front;

    return *this;
  }

  CameraComponent& CameraComponent::updateViewMatrix() {
    view_matrix = glm::lookAt(position, position + front, up);
    return *this;
  }

  glm::mat4& CameraComponent::getView() {
    updateViewMatrix();
    return view_matrix;
  }

  glm::vec3 CameraComponent::getPosition() {
    return position;
  }
  glm::vec3 CameraComponent::getUp() {
    return up;
  }
  glm::vec3 CameraComponent::getFront() {
    return front;
  }

  CameraComponent& CameraComponent::setPosition(glm::vec3 p) {
    position = p;
    return *this;
  }
  CameraComponent& CameraComponent::setUp(glm::vec3 u) {
    up = u;
    return *this;
  }
  CameraComponent& CameraComponent::setFront(glm::vec3 f) {
    front = f;
    return *this;
  }

  // Projection Matrix
  CameraComponent& CameraComponent::setProjectionMatrix(float f, float np,
    float fp, float width, float height) {

    type = CameraType::Perspective;
    fov = f;
    near_plane = np;
    far_plane = fp;
    this->width = width;
    this->height = height;

    return *this;
  }

  CameraComponent& CameraComponent::setOrthoMatrix(float l,
    float r, float b, float t, float np, float fp) {

    type = CameraType::Orthographic;
    left = l;
    right = r;
    bottom = b;
    top = t;
    far_plane = fp;
    near_plane = np;

    return *this;
  }

  glm::mat4& CameraComponent::getProjection() {
    return projection_matrix;
  }


  CameraComponent& CameraComponent::setFov(float f) {
    fov = f;
    return *this;
  }
  CameraComponent& CameraComponent::setNearPlane(float np) {
    near_plane = np;
    return *this;
  }
  CameraComponent& CameraComponent::setFarPlane(float fp) {
    far_plane = fp;
    return *this;
  }

  void SetMainCamera(Entity c) {
    Engine::get()->main_camera = c;
  }

}