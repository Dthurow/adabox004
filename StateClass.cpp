#include "StateClass.h"
#include "Arduino.h"


State::State(bool isPaused, uint8_t volume, int lastRemoteVal){
	IsPaused = isPaused;
	Volume = volume;
	LastRemoteVal = lastRemoteVal;
}