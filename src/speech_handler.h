#pragma once

#include <AudioTools.h>
#include <FileSystems.h>
#include <espeak.h>
#include <SPI.h>

class speech_handler {
public:
	speech_handler(AnalogAudioStream& analog);
	~speech_handler();
	void speak(const std::string& text, bool interrupt=false);

private:
	ESpeak espeak;
};
