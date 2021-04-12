// Author: Román Chérrez Mala
// Email: cherrezmar@coventry.ac.uk

#ifndef _TIMER_H_
#define _TIMER_H_ 1

namespace Aloe {
  class Timer {

  public:
    /** Default constructor*/
    Timer();
    /** Default deconstructor*/
    ~Timer();

    /** Start the timer*/
    void StartTimer();

    /** Gets time passed*/
    double GetTime();

  private:

    /** Internal data structure*/
    struct Data;
    Data* data;
  };

  /** Function to get the time since the start of the application*/
  double GetTimeSinceStart();
}

#endif // _TIMER_H_
