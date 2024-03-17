#include <TTS.h>

#define PWM D2

TTS text2speech(PWM);  

void setup() { 
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  text2speech.setPitch(6);
  text2speech.sayText("Hello  master! How are you doin?");
  delay(500); 

  text2speech.setPitch(1);
  text2speech.sayText("I am fine, thankyou."); 

  delay(1000);
}  

