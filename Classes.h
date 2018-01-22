#include <Adafruit_VS1053.h>

#ifndef HEADER_STATECLASS
  #define HEADER_STATECLASS
   
  class Environment{
  private:
  	char _buffer[10];
	int _bufferIndex;
	int LastRemoteVal;
	boolean IsPaused;
	uint8_t Volume;
	void AddToBuffer(char x);
	String ReadBuffer();
  public:
  	Environment(bool isPaused, uint8_t volume, int lastRemoteVal);
	Adafruit_VS1053_FilePlayer MusicPlayer;
	void ProcessInput(int inputVal);
	void DoCurrentActionState();
	
  };


  class State {
  public:
  	void (*StateAction)

  }

#endif