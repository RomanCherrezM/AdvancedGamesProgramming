// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Graphics/shader.h"
#include "glad/glad.h"

#include <fstream>
#include <vector>
#include "scoped_array.h"

namespace Aloe {

  struct Shader::Data {
    u32 shader;

    ShaderType type;
    std::string path;
  };

  Shader::Shader() {
    data = new Data();

    data->shader = -1;
  }

  Shader::~Shader() {
    glDeleteShader(data->shader);
  }

  static GLenum translate(ShaderType type) {
  
    switch (type) { 
    case ShaderVertex:
      return GL_VERTEX_SHADER;
    case ShaderFragment:
      return GL_FRAGMENT_SHADER;
    case ShaderGeometry:
      return GL_GEOMETRY_SHADER;
    default:  
      return GL_NONE;
    } 
  }

  bool ReadShader(std::string path, scoped_array<char> *buffer) {
    //File reading
    std::ifstream test(path);
    if (!test) {
      printf("\n[ERROR] [SHADER] : Not existing path");
    }

    FILE* fp;
    fopen_s(&fp, path.c_str(), "rb");
    if (fp == nullptr)return false;

    fseek(fp, 0, SEEK_END);
    int lenght = ftell(fp);
    rewind(fp);
    buffer->alloc(lenght + 1);

    fread(buffer->get(), lenght, 1, fp);
    *(buffer->get() + lenght) = '\0';
    fclose(fp);

    return true;
  }

  bool CompileShader(u32 shader, const char* data) {

    //Set shader data
    glShaderSource(shader, 1, &(const GLchar*)data, NULL);

    //Compile and check Error
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> errorLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

      printf("\n [ERROR] [SHADER] : Compile error\n %s", &errorLog[0]);

      // Exit with failure.
      glDeleteShader(shader);
      return false;
    }
    printf("\n [SHADER] : Compile success");
    return true;
  }

  bool Shader::create(std::string path, ShaderType type) {

    data->shader = glCreateShader(translate(type));
    data->type = type;
    data->path = path;

    scoped_array<char> buffer;
    ReadShader(path, &buffer);

    const char* buffer_data = buffer.get();
    return CompileShader(data->shader, buffer_data);
  }

  bool Shader::recompile() {
    if (data->shader == -1)return false;
    glDeleteShader(data->shader);
     
    data->shader = glCreateShader(translate(data->type));

    scoped_array<char> buffer;
    ReadShader(data->path, &buffer);

    const char* buffer_data = buffer.get();

    return CompileShader(data->shader, buffer_data);
  }

  bool Shader::valid(){
    return data->shader != -1;
  }

  u32 Shader::get(){
    return data->shader;
  }
}
