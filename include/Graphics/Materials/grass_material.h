// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _GRASS_MATERIAL_H_
#define _GRASS_MATERIAL_H_ 1

#include "Graphics/Materials/base_material.h"
#include "Graphics/texture.h"

namespace Aloe {

  class GrassMaterial : public BaseMaterial {
  public:
    GrassMaterial();
    ~GrassMaterial();

    virtual void setupUniforms() override;
    virtual void use(TransformComponent* tr = nullptr) override;

    Texture texture;
  };
}

#endif