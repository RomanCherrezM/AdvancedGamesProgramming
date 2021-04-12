// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _BASE_MATERIAL_H_
#define _BASE_MATERIAL_H_ 1

#include "Graphics/shader.h"
#include "Core/platform_types.h"
#include "ECS/Components/render_component.h"
#include "ECS/Components/transform_component.h"

#include <map>

typedef u32 MaterialHandler;

namespace Aloe {

  enum DrawMode {

    Triangles,
    Points
      
  };

  class BaseMaterial {
  public:
    BaseMaterial();
    ~BaseMaterial();

    /** Creates the material with the given shaders, linking them*/
    void create(Shader vertex, Shader fragment);
    /** Creates the material with the given shaders, linking them*/
    void create(Shader vertex, Shader fragment,Shader geometry);
    /** Recompiles the shaders and links the program again*/
    void recompileShaders();
    /** Function to implement, must fill the uniforms array with the OpenGL
    * uniform location for further use*/
    virtual void setupUniforms();
    /** Setups the OpenGL program and the current and changes values 
    * depending on the material, also passes all the data to the shaders*/
    virtual void use(TransformComponent* tr = nullptr);

    /** Draw mode setter, default is Triangles*/
    void setDrawMode(DrawMode drawmode);
    /** Draw mode getter*/
    DrawMode getDrawMode();

    /** Sets the material wireframe state*/
    void setWireframe(bool w);
    /** gets the material wireframe state*/
    bool getWireframe();

    /** Gets the material internal handler*/
    MaterialHandler getHandler();

    /**Uniforms data*/
    std::map<const char*, u32> uniforms;
    
  private:
    //Internal Data
    struct Data;
    Data* data;
  };
}

#endif