// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _WINDOW_H_
#define _WINDOW_H_ 1

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Aloe {

  /** Window class*/
  class Window {

  public:
    /** Default constructor*/
    Window();
    /** Default deconstructor*/
    ~Window();

    /** Window create function*/
    void create(int width, int height, const char* title);
    /** Returns if window should close*/
    bool shouldClose();
    /** Updates the window internal buffers and poll events*/
    void update();
    /** Terminates the current frame*/
    void endFrame();
    /** Width getter*/
    int getWidth();
    /** Height getter*/
    int getHeight();
    /** Returns if the window is valid*/
    bool isValid();

    /**  GLFW native window getter*/
    GLFWwindow* getNativeWindow();

  private:

    /**Internal window data*/
    struct Data;
    Data* data = nullptr;

  };

  /** Function to create the window context of the app, Use this if only one window is openned*/
  void CreateWindowContext(int width,int height, const char* title);

}

#endif // _WINDOW_H_
