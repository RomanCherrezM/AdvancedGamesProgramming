// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Graphics/texture.h"
#include "scoped_array.h"
#include "glad/glad.h"
#include "Core/engine.h"

namespace Aloe {

  struct Texture::Data {
    TextureHandler handler;
  };

  Texture::Texture() {
    data = new Data();
    data->handler = -1;
  }

  Texture::~Texture() {
  }

  Texture& Texture::create() {
    //Handler
    ResourceManager* r = &Engine::get()->resource_manager;
    if (data->handler == -1) {
      data->handler = r->newTexture();
    }

    return *this;
  }

  Texture& Texture::loadTexture(const char* src){
    Engine::get()->resource_manager.loadTexture(data->handler,src);    
    return *this;
  }

  Texture& Texture::loadEmpty(int width, int height){
    Engine::get()->resource_manager.loadEmpty(data->handler, width,height);
    return *this;
  }

  Texture& Texture::resize(int width, int height){
    Engine::get()->resource_manager.loadEmpty(data->handler, width, height);
    return *this;
  }

  Texture& Texture::loadCubemap(std::vector<std::string> path){
    Engine::get()->resource_manager.loadCubemap(data->handler, path);
    return *this;
  }

  Texture& Texture::setFormat(Format tex_format){
    Engine::get()->resource_manager.setFormat(data->handler, tex_format);
    return *this;
  }

  Texture& Texture::setType(Type tex_type){
    Engine::get()->resource_manager.setType(data->handler, tex_type);
    return *this;
  }

  Texture& Texture::setFilter(Filter tex_filter){
    Engine::get()->resource_manager.setFilter(data->handler, tex_filter);
    return *this;
  }

  Texture& Texture::setWrap(Wrap tex_wrap){
    Engine::get()->resource_manager.setWrap(data->handler, tex_wrap);
    return *this;
  }

  TextureHandler Texture::getHandler(){
    return data->handler;
  }
  u32 Texture::getInternalHandler(){
    return Engine::get()->resource_manager.textures[data->handler].texture;
  }
}
