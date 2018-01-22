#include "Classes.h"
#include "Arduino.h"

// These are the pins used
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)
#define VS1053_CS      16     // VS1053 chip select pin (output)
#define VS1053_DCS     15     // VS1053 Data/command select pin (output)
#define CARDCS          2     // Card chip select pin
#define VS1053_DREQ     0     // VS1053 Data request, ideally an Interrupt pin

Environment::Environment(bool isPaused, uint8_t volume, int lastRemoteVal){
	IsPaused = isPaused;
	Volume = volume;
	LastRemoteVal = lastRemoteVal;
	musicPlayer =   Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);
}

Environment::ProcessInput(int inputVal){
	
	//if this is a number key, add to the buffer and return

	//deal with repeat codes

	//set the lastRemoteVal to this value

	//set the new state (if it's changed)
}

Environment::DoCurrentActionState(){
	//trigger the current state's action
	//for debug purporses, also print out the current state's name
}