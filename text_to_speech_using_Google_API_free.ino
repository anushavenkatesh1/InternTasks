#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"

#define I2S_DOUT      25
#define I2S_BCLK      26
#define I2S_LRC       27

Audio audio;


void setup()
{

  Serial.begin(115200);

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin( "cvsratnam", "cvsratnam123");

  while (WiFi.status() != WL_CONNECTED)
    delay(100);

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(100);
  audio.connecttospeech("TIME TO TAKE MEDICINE DOLO", "en"); 
  delay(100);


}

void loop()
{

  audio.loop();

}

void audio_info(const char *info) {
  Serial.print("audio_info: "); Serial.println(info);
}
