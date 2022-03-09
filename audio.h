#ifndef _SOUND_H
#define _SOUND_H

#include <SDL2/SDL.h>
#include "enum.h"
#include <stdio.h>

#ifndef BYTE_TYPE_DEFS
#define BYTE_TYPE_DEFS
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
#endif

class AUDIO
{
private:
	//NR10
	int ch1SweepTime;
	int ch1SweepOP;
	int ch1SweepShift;
	//NR11
	int ch1WaveDuty;
	int ch1SoundLength; //write only
	//NR12
	int ch1VolumeEnvelope;
	int ch1EnvelopeChange;
	int ch1EnvelopeSweep;
	//NR13 - NR14(2-0)
	u16 ch1Freq; //write only
	//NR14
	int ch1Restart; //write only
	int ch1Counter;

	//NR21
	int ch2WaveDuty;
	int ch2SoundLength; //write only
	//NR22
	int ch2VolumeEnvelope;
	int ch2EnvelopeChange;
	int ch2EnvelopeSweep;
	//NR23 - NR24(2-0)
	u16 ch2Freq; //write
	//NR24
	int ch2Restart;
	int ch2Counter;

	//NR30
	int ch3On;
	//NR31
	int ch3SoundLength; //write only
	//NR32
	int ch3OutputLevel;
	//NR23 - NR24(2-0)
	u16 ch3Freq; //write
	//NR24
	int ch3Restart;
	int ch3Counter;
	//Wave Pattern Ram (FF30 - FF3F)
	u8 WavePattern[16];

	//NR41
	int ch4SoundLength;
	//NR42
	int ch4VolumeEnvelope;
	int ch4EnvelopeChange;
	int ch4EnvelopeSweep;
	//NR43
	int ch4ShiftClock;
	int ch4CounterWidth;
	int ch4DivRatio;
	//NR44
	int ch4Restart; //write
	int ch4Counter;

	//NR50
	int enableVINtoSO2;
	int SO2outLevel;
	int enableVINtoSO1;
	int SO1outLevel;
	//NR51
	int ch4toSO2;
	int ch3toSO2;
	int ch2toSO2;
	int ch1toSO2;
	int ch4toSO1;
	int ch3toSO1;
	int ch2toSO1;
	int ch1toSO1;
	//NR52
	int soundON;
	int ch4ON; //read
	int ch3ON; //read
	int ch2ON; //read
	int ch1ON; //read

	int initSuccess;
	SDL_AudioSpec want,spec;
	SDL_AudioDeviceID device;


public:
	AUDIO();
	~AUDIO();

	int init();
	void reset();

	u8 read(u8 addr);
	void write(u8 addr, u8 val);

	void tick(int cycle);
	void updateStream(void* userdata, Uint8* stream, int len);

	void close();
};

#endif