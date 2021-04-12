// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _TERRAIN_MATERIAL_H_
#define _TERRAIN_MATERIAL_H_ 1

#include "Graphics/Materials/base_material.h"
#include "Graphics/texture.h"

namespace Aloe {

  class TerrainMaterial : public BaseMaterial {
  public:
    TerrainMaterial();
    ~TerrainMaterial();

    virtual void setupUniforms() override;
    virtual void use(TransformComponent* tr = nullptr) override;

    Texture base_texture;
    Texture grass_texture;
    Texture sand_texture;
  };
}

#endif