#include <Adafruit_VS1053.h>

#ifndef HEADER_STATECLASS
  #define HEADER_STATECLASS
   
  class State{
  public:
  	State(bool isPaused, uint8_t volume, int lastRemoteVal);
  	boolean IsPaused;
	uint8_t Volume;
	int LastRemoteVal;
  };


#endif