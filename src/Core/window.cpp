// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Core/window.h"
#include "Core/engine.h"
#include "Core/window.h"

#include "glad/glad.h"

namespace Aloe {
  struct Window::Data {
    GLFWwindow* window = NULL;

    int width;
    int height;
  };
  //Internal window data

  Window::Window() {
    data = new Data;
  }

  Window::~Window() {

    glfwDestroyWindow(data->window);
    glfwTerminate();

    delete data;
    data = nullptr;
  }

  GLFWwindow* Window::getNativeWindow() {
    return data->window;
  }

  void Window::create(int width, int height, const char* title){

    data->width = width;
    data->height = height;

    /* create window and GL context via GLFW */
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (glfwInit()) {
      data->window = glfwCreateWindow(width, height, "Aloe Engine", nullptr, nullptr);
    }
    glfwSetInputMode(data->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(data->window);
    gladLoadGL();
  }

  bool Window::shouldClose(){
    return glfwWindowShouldClose(data->window);
  }

  void Window::update(){
    glfwPollEvents();
    glfwGetFramebufferSize(data->window, &data->width, &data->height);
  }

  void Window::endFrame() {
    glfwSwapBuffers(data->window);
  }

  int Window::getWidth(){
    return data->width;
  }

  int Window::getHeight() {
    return data->height;
  }

  bool Window::isValid(){
      return data->window != NULL;
  }

  void CreateWindowContext(int width, int height, const char* title) {  
    Engine* e = Engine::get();

    e->app.window.create(width,height,title);
  }
}