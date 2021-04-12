// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _SHADER_H_
#define _SHADER_H_ 1

#include "Core/platform_types.h"

#include <string>

namespace Aloe {

  enum ShaderType {

    ShaderVertex,
    ShaderFragment,
    ShaderGeometry

  };

  class Shader {
  public:
    Shader();
    ~Shader();

    /** Creates and compiles the shader given a path and a type*/
    bool create(std::string path, ShaderType type);
    /** Recompiles the shader*/
    bool recompile();
    /** Return if the shader is valid*/
    bool valid();
    /** Gets the shader*/
    u32 get();

  private:

    // Internal Data
    struct Data;
    Data* data;  
  };
}

#endif