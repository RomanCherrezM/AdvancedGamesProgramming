// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _COLOR_MATERIAL_H_
#define _COLOR_MATERIAL_H_ 1

#include "Graphics/Materials/base_material.h"

namespace Aloe {

  class ColorMaterial : public BaseMaterial {
  public:
    ColorMaterial();
    ~ColorMaterial();

    virtual void setupUniforms() override;
    virtual void use(TransformComponent* tr = nullptr) override;

    glm::vec4 color;
  };

  ColorMaterial* CreateColorMaterial(glm::vec4 color);

}

#endif