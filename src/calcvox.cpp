#include "AudioTools.h" // https://github.com/pschatzmann/arduino-audio-tools
//#include "AudioLibs/AudioKit.h" // https://github.com/pschatzmann/arduino-audiokit
#include "FileSystems.h" // https://github.com/pschatzmann/arduino-posix-fs
#include "espeak.h"
#include "SPI.h"

AnalogAudioStream analog ; // or replace with AudioKitStream for AudioKit
ESpeak espeak(analog);

void setup() {
  Serial.begin(115200);
  //file_systems::FSLogger.begin(file_systems::FSInfo, Serial); 

  // setup espeak
  espeak.begin();

  // setup output
  audio_info espeak_info = espeak.audioInfo();
  auto cfg = analog.defaultConfig();
  analog.begin(cfg);

}

void loop() {
  espeak.say("Hello world!");
  delay(5000);
}