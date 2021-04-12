// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _TEXTURE_H_
#define _TEXTURE_H_ 1

#include "Core/platform_types.h"
#include <vector>
#include <string>

namespace Aloe {

  enum Type {
    T_Invalid,
    T_1D,
    T_2D,
    T_3D,
    T_CUBEMAP
  };

  enum Format {
    F_None,
    F_R,
    F_RG,
    F_RGB,
    F_BGR,
    F_BGRA,
    F_RGBA,
    F_SRGB,
    F_UNORM,
    F_DEPTH,
    F_DEPTH_COMPONENT,
    F_DEPTH32,
  };

  enum Filter {
    F_NEAREST,
    F_LINEAR,
    F_NEAREST_MIPMAP_NEAREST,
    F_LINEAR_MIPMAP_NEAREST,
    F_NEAREST_MIPMAP_LINEAR,
    F_LINEAR_MIPMAP_LINEAR,
  };

  enum Wrap {
    W_REPEAT,
    W_MIRRORED_REPEAT,
    W_CLAMP_TO_EDGE,
  };

  typedef u32 TextureHandler;

  class Texture {
  public:
    Texture();
    ~Texture();

    /** Creates the texture*/
    Texture& create();

    /** Loads the texture given a path*/
    Texture& loadTexture(const char* src);
    /** Load an empty texture*/
    Texture& loadEmpty(int width, int height);
    /** Resizes the texture*/
    Texture& resize(int width, int height);
    /** Load a cubemap texture given a vector of paths to the cubemap faces*/
    Texture& loadCubemap(std::vector<std::string> path);
    /** Sets the texture forma*/
    Texture& setFormat(Format tex_format);
    /** Sets the texture type*/
    Texture& setType(Type tex_type);
    /** Sets the texture filter*/
    Texture& setFilter(Filter tex_filter);
    /** Sets the texture wrap*/
    Texture& setWrap(Wrap tex_wrap);

    /** Texture handler getter*/
    TextureHandler getHandler();
    /** OpenGl handler getter*/
    u32 getInternalHandler();

  private:

    // Internal Data
    struct Data;
    Data* data;  
  };
}

#endif