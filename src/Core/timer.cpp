// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#include "Core/timer.h"
#include "Core/engine.h"

#include <stdio.h>
#include <ctime>

namespace Aloe {

  struct Timer::Data {
    std::clock_t start_time;
    std::clock_t end_time;
  };

  Timer::Timer() {
    data = new Data;
  }
  Timer::~Timer() {
    delete data;
  }

  void Timer::StartTimer() {    
    data->start_time = std::clock();
  }

  double Timer::GetTime() {
    data->end_time = std::clock();
    double time_passed = (1000.0 * (data->end_time - data->start_time)) / CLOCKS_PER_SEC;

    return time_passed;
  }

  double GetTimeSinceStart() {
  
    return Engine::get()->timer.GetTime();
  }

}