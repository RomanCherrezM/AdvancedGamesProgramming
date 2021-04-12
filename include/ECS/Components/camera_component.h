// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _CAMERA_COMPONENT_H_
#define _CAMERA_COMPONENT_H_ 1

#include "vec3.hpp"
#include "mat4x4.hpp"

#include "ECS/ecs_constants.h"

namespace Aloe {
  class CameraComponent {
  public:

    CameraComponent();

    enum CameraType {
      Perspective,
      Orthographic
    };

    //Common
    CameraType type;

    //View Matrix
    CameraComponent& setViewMatrix(glm::vec3 position, glm::vec3 front,
    glm::vec3 up);
    glm::mat4& getView();

    glm::vec3 getPosition();
    glm::vec3 getUp();
    glm::vec3 getFront();

    CameraComponent& setPosition(glm::vec3 p);
    CameraComponent& setUp(glm::vec3 u);
    CameraComponent& setFront(glm::vec3 f);

    //Projection Matrix
    CameraComponent& updateViewMatrix();
    CameraComponent& setProjectionMatrix(float f,
      float np, float fp, float width,
      float height);
    CameraComponent& setOrthoMatrix(float l,
      float r, float b, float t, float np, float fp);
    glm::mat4& getProjection();

    CameraComponent& setFov(float f);
    CameraComponent& setNearPlane(float np);
    CameraComponent& setFarPlane(float fp);

    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 orthographic_matrix;

    // View Matrix

    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 front;

    // Projection Matrix

    float width;
    float height;
    float fov;

    //Ortho Matrix
    float left;
    float right;
    float bottom;
    float top;

    //Both
    float near_plane;
    float far_plane;

  };

  void SetMainCamera(Entity c);

}
#endif