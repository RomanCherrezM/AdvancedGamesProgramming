#ifndef _APP_H_
#define _APP_H_ 1

#include "window.h"

namespace Aloe {

  class App {

  public:
    App() {}
    ~App() {}

    /** Engine start*/
    void EngineStart();
    /** Engine loop*/
    void EngineLoop();
    /** Engine close*/
    void EngineClose();

    Window window;
  };

}

#endif