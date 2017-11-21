// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <sstream>

int RECV_PIN = 4; //an IR detector/demodulatord is connected to GPIO pin 2
IRrecv irrecv(RECV_PIN);

// These are the pins used
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)
#define VS1053_CS      16     // VS1053 chip select pin (output)
#define VS1053_DCS     15     // VS1053 Data/command select pin (output)
#define CARDCS          2     // Card chip select pin
#define VS1053_DREQ     0     // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

// the name of what we're going to play
char foundname[20];
boolean isPaused = false;
uint8_t volume = 10;
int lastRemoteVal = 0;

void setup() {
  Serial.begin(115200);

  Serial.println("\n\nAdafruit VS1053 Feather Test");
  
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }

  Serial.println(F("VS1053 found"));
 
  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial.println("SD OK!");
  
  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(volume,volume);

  // If DREQ is on an interrupt pin we can do background
  // audio playing
 // musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Playing one Track");
}

void loop() {
  decode_results results;

 if(digitalRead(VS1053_DREQ) && !musicPlayer.stopped() && !isPaused) {
    musicPlayer.feedBuffer();
  } 

  // look for a message!
  if (irrecv.decode(&results)) {
    irrecv.resume(); // Receive the next value

    //print out the current results value
    //have to do the cast because otherwise it freaks out about printing a uint64_t :(
    Serial.print("\nThe Value is: 0x");
    Serial.println((int)(results.value), HEX);

    // handle repeat codes. If it's a repeat code, and the last time it was
    //the volume up or down, don't erase the lastRemoteVal, let that 
    //volume up or down flow through so it changes the volume again 
    //but otherwise, wipe out the lastRemoteVal
    if ((int)results.value != -1 || 
          (lastRemoteVal != 0xFD40BF && 
              lastRemoteVal != 0xFD00FF)
       ) {
      lastRemoteVal = results.value;
    }





    //gigantic if/else statement \(^_^)/
    if (lastRemoteVal == 0xFD08F7) {
       musicPlayer.stopPlaying();
       Serial.println("playing track #1");
       musicPlayer.startPlayingFile("track001.mp3");
    }
    else if (lastRemoteVal == 0xFD8877) {
       musicPlayer.stopPlaying();
       Serial.println("playing track #2");
       musicPlayer.startPlayingFile("track002.mp3");
    }
    else if (lastRemoteVal == 0xFD48B7) {
       musicPlayer.stopPlaying();
       Serial.println("playing track #3");
       musicPlayer.startPlayingFile("track003.mp3");
    }
    else if (lastRemoteVal == 0xFD40BF) { //vol+
      Serial.println("Vol+");
      if (volume > 0) {
         volume--;
         musicPlayer.setVolume(volume,volume);
      }
    }
    else if (lastRemoteVal == 0xFD00FF) { //vol-
      Serial.println("Vol-");
      if (volume < 100) {
         volume++;
         musicPlayer.setVolume(volume,volume);
      }
    }
    else if (lastRemoteVal == 0xFD807F) { // playpause
      Serial.println("Play/Pause");
      isPaused = !isPaused; // toggle!
    } 
  }

  delay(1);
}


boolean findFileStartingWith(char *start) {
  File root;
  root = SD.open("/");
  root.rewindDirectory();
  while (true) {
    File entry =  root.openNextFile();
    if (! entry) {
      return false;
    }
    String filename = entry.name();
    Serial.print(filename);
    if (entry.isDirectory()) {
      Serial.println("/");
    } else {
      Serial.println();
      if (filename.startsWith(start)) {
        filename.toCharArray(foundname, 20); 
        entry.close();
        root.close();
        return true;
      }
    }
    entry.close();
  }
}


/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}



