// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _WATER_MATERIAL_H_
#define _WATER_MATERIAL_H_ 1

#include "Graphics/Materials/base_material.h"
#include "Graphics/texture.h"

namespace Aloe {

  class WaterMaterial : public BaseMaterial {
  public:
    WaterMaterial();
    ~WaterMaterial();

    virtual void setupUniforms() override;
    virtual void use(TransformComponent* tr = nullptr) override;

    Texture height_map1;
    Texture height_map2;
    Texture normal_map1;
    Texture normal_map2;
    Texture cubemap;
  };
}

#endif