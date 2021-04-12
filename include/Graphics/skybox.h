// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _SKYBOX_H_
#define _SKYBOX_H_ 1

#include "Core/platform_types.h"
#include <vector>
#include <string>
#include "Graphics/Materials/skybox_material.h"

namespace Aloe {

  class Skybox {
  public:
    Skybox();
    ~Skybox();

    /** Create the skybox given a vector of paths to the different cubemap faces*/
    Skybox& create(std::vector<std::string> path);
    /** Gets the cubemap texture*/
    Texture& getTexture();
    /** Sets the skybox as the current*/
    void setAsCurrent();


  private:
    //Internal data
    struct Data;
    Data* data;
  };
}

#endif