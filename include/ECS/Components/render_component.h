// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _RENDER_COMPONENT_H_
#define _RENDER_COMPONENT_H_ 1

#include "Graphics/mesh.h"
#include "Graphics/Materials/base_material.h"

#include "px_mem.h"

namespace Aloe {

  class BaseMaterial;

  class RenderComponent {
  public:
    RenderComponent();
    ~RenderComponent();

    Mesh *mesh;
    BaseMaterial *material;
  };
}

#endif
