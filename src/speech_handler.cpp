#include "speech_handler.h"

speech_handler::speech_handler(AnalogAudioStream& analog) : espeak(analog) {
	espeak.begin();
	auto espeak_info = espeak.audioInfo();
	auto cfg = analog.defaultConfig();
	cfg.channels = espeak_info.channels; // 1
	cfg.sample_rate = espeak_info.sample_rate; // 22050
	cfg.bits_per_sample = espeak_info.bits_per_sample; // 16
	analog.begin(cfg);
}

speech_handler::~speech_handler() {
	
}

void speech_handler::speak(const std::string& text, bool interrupt) {
	espeak.say(text);
}
