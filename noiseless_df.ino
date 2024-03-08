#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"

HardwareSerial mySoftwareSerial(1);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  mySoftwareSerial.begin(9600, SERIAL_8N1, 5, 18);  

  Serial.begin(115200);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(myDFPlayer.readType(), HEX);
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1. Please recheck the connection!"));
    Serial.println(F("2. Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500);

  myDFPlayer.volume(1);  // Set volume value (0~30).
  myDFPlayer.volumeUp(); 
  myDFPlayer.volumeDown(); 

  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  int delayms = 100;

  Serial.println(F("readState--------------------"));
  Serial.println(myDFPlayer.readState());
  Serial.println(F("readVolume--------------------"));
  Serial.println(myDFPlayer.readVolume());

  Serial.println(F("readFileCounts--------------------"));
  Serial.println(myDFPlayer.readFileCounts());
  Serial.println(F("readCurrentFileNumber--------------------"));
  Serial.println(myDFPlayer.readCurrentFileNumber());
  Serial.println(F("readFileCountsInFolder--------------------"));
  Serial.println(myDFPlayer.readFileCountsInFolder(3));
  Serial.println(F("--------------------"));

  Serial.println(F("myDFPlayer.play(1)"));
  delay(1000); 
  myDFPlayer.play(1);
  delay(1000); 
}

void loop() {

}
