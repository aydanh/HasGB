#include "audio.h"

AUDIO::AUDIO(){}
AUDIO::~AUDIO(){}

int AUDIO::init()
{
	if(SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
	{
		printf("Could not initialize Audio!\n");
		initSuccess = 0;
		return 0;
	}else{ initSuccess = 1; return 1;}

	want.freq = 44100;
	want.samples = 1024;
	want.format = AUDIO_S16SYS;
	want.channels = 2;
//	want.callback = setStream;
	want.userdata = NULL;

	device = SDL_OpenAudioDevice(NULL, 0,&want, &spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	if(device == 0)
	{
		printf("Could not open audio device: %s\n", SDL_GetError());
		initSuccess = 0;
		return 0;
	}
	else
	{
	}

}

void AUDIO::reset()
{
	ch1SweepTime = 0;
	ch1SweepOP = 0;
	ch1SweepShift = 0;
	ch1WaveDuty = 0x2;
	ch1SoundLength = 0x3F;
	ch1VolumeEnvelope = 0xF;
	ch1EnvelopeChange = 0;
	ch1EnvelopeSweep = 0x3;
	ch1Freq = 0x700;
	ch1Restart = 1;
	ch1Counter = 0;

	ch2WaveDuty = 0;
	ch2SoundLength = 0x3F;
	ch2VolumeEnvelope = 0;
	ch2EnvelopeChange = 0;
	ch2EnvelopeSweep = 0;
	ch2Freq = 0x77F;
	ch2Restart = 1;
	ch2Counter = 0;

	ch3On = 0;
	ch3SoundLength = 0xFF;
	ch3OutputLevel = 0;
	ch3Freq = 0xBF;
	ch3Restart = 0;
	ch3Counter = 0;
	for(int i=0; i<16; i++) { WavePattern[i] = 0; }

	ch4SoundLength = 0x3F;
	ch4VolumeEnvelope = 0;
	ch4EnvelopeChange = 0;
	ch4EnvelopeSweep = 0;
	ch4ShiftClock = 0;
	ch4CounterWidth = 0;
	ch4DivRatio = 0;
	ch4Restart = 1;
	ch4Counter = 0;

	enableVINtoSO2 = 0;
	SO2outLevel = 0x7;
	enableVINtoSO1 = 0;
	SO1outLevel = 0x7;

	ch4toSO2 = 1;
	ch3toSO2 = 1;
	ch2toSO2 = 1;
	ch1toSO2 = 1;
	ch4toSO1 = 0;
	ch3toSO1 = 0;
	ch2toSO1 = 1;
	ch1toSO1 = 1;
	
	ch4ON = 1;
	ch3ON = 0;
	ch2ON = 0;
	ch1ON = 0;
}

u8 AUDIO::read(u8 addr)
{
	switch(addr)
	{
		case NR10:
		{
			u8 n = 0x80;
			n |= (ch1SweepTime<<4);
			n |= (ch1SweepOP<<3);
			n |= (ch1SweepShift);
			return n;
		}
		break;

		case NR11:
		{
			u8 n;
			n |= (ch1WaveDuty<<6);
			n |= 0x1F;
			return n;
		}
		break;

		case NR12:
		{
			u8 n;
			n |= (ch1VolumeEnvelope<<4);
			n |= (ch1EnvelopeChange<<3);
			n |= (ch1EnvelopeSweep);
			return n;
		}
		break;

		case NR13:
		{
			return 0xFF;
		}
		break;

		case NR14:
		{
			u8 n = 0xBF;
			n |= (ch1Counter<<6);
			return n;
		}
		break;

		case NR21:
		{
			u8 n;
			n |= (ch2WaveDuty<<6);
			n |= 0x1F;
			return n;
		}
		break;

		case NR22:
		{
			u8 n;
			n |= (ch2VolumeEnvelope<<4);
			n |= (ch2EnvelopeChange<<3);
			n |= (ch2EnvelopeSweep);
			return n;
		}
		break;

		case NR23:
		{
			return 0xFF;
		}
		break;

		case NR24:
		{
			u8 n = 0xBF;
			n |= (ch2Counter<<6);
			return n;
		}
		break;

		case NR30:
		{
			return (ch3On<<7)|0x7F;
		}
		break;

		case NR31:
		{
			return ch3SoundLength;
		}
		break;

		case NR32:
		{
			u8 n = 0x9F;
			n |= (ch3OutputLevel<<5);
			return n;
		}
		break;

		case NR33:
		{
			return 0xFF;
		}
		break;

		case NR34:
		{
			u8 n = 0xBF;
			n |= (ch3Counter<<6);
			return n;
		}
		break;

		case NR41:
		{
			u8 n = 0xC0;
			n |= ch4SoundLength;
			return n;
		}
		break;

		case NR42:
		{
			u8 n;
			n |= (ch4VolumeEnvelope<<4);
			n |= (ch4EnvelopeChange<<3);
			n |= (ch4EnvelopeSweep);
			return n;
		}
		break;

		case NR43:
		{
			u8 n;
			n |= (ch4ShiftClock<<4);
			n |= (ch4CounterWidth<<3);
			n |= (ch4DivRatio);
			return n;
		}
		break;

		case NR44:
		{
			u8 n = 0xBF;
			n |= (ch4Counter<<6);
			return n;
		}
		break;

		case NR50:
		{
			u8 n;
			n |= (enableVINtoSO2<<7);
			n |= (SO2outLevel<<4);
			n |= (enableVINtoSO1<<3);
			n |= (SO1outLevel);
			return n;
		}
		break;

		case NR51:
		{
			u8 n;
			n |= (ch4toSO2<<7);
			n |= (ch3toSO2<<6);
			n |= (ch2toSO2<<5);
			n |= (ch1toSO2<<4);
			n |= (ch4toSO1<<3);
			n |= (ch3toSO1<<2);
			n |= (ch2toSO1<<1);
			n |= (ch1toSO1);
			return n;
		}
		break;

		case NR52:
		{
			u8 n = 0x70;
			n |= (soundON<<7);
			n |= (ch4ON<<3);
			n |= (ch3ON<<2);
			n |= (ch2ON<<1);
			n |= (ch1ON);
			return n;
		}
		break;	
	}
}

void AUDIO::write(u8 addr, u8 val)
{
	switch(addr)
	{
		case NR10:
		{
			ch1SweepTime = (val&0x70)>>4;
			ch1SweepOP  = (val&0x08)?1:0;
			ch1SweepShift = (val&0x07);
		}
		break;

		case NR11:
		{
			ch1WaveDuty = (val&0xC0)>>6;
			ch1SoundLength = (val&0x3F);
		}
		break;

		case NR12:
		{
			ch1VolumeEnvelope = (val&0xF0)>>4;
			ch1EnvelopeChange = (val&0x08)?1:0;
			ch1EnvelopeSweep = (val&0x07);
		}
		break;

		case NR13:
		{
			ch1Freq = (ch1Freq&0xF00) + val;
		}
		break;

		case NR14:
		{
			ch1Restart = (val&0x80)?1:0;
			ch1Counter = (val&0x40)?1:0;
			ch1Freq = (ch1Freq&0xFF) + ((val&0x7)<<8);
		}
		break;

		case NR21:
		{
			ch2WaveDuty = (val&0xC0)>>6;
			ch2SoundLength = (val&0x3F);
		}
		break;

		case NR22:
		{
			ch2VolumeEnvelope = (val&0xF0)>>4;
			ch2EnvelopeChange = (val&0x08)?1:0;
			ch2EnvelopeSweep = (val&0x07);
		}
		break;

		case NR23:
		{
			ch2Freq = (ch2Freq&0xF00) + val;
		}
		break;

		case NR24:
		{
			ch2Restart = (val&0x80)?1:0;
			ch2Counter = (val&0x40)?1:0;
			ch2Freq = (ch2Freq&0xFF) + ((val&0x7)<<8);
		}
		break;

		case NR30:
		{
			ch3On = (val&0x80)?1:0;
		}
		break;

		case NR31:
		{
			ch3SoundLength = val;
		}
		break;

		case NR32:
		{
			ch3OutputLevel = (val&0x60)>>5;
		}
		break;

		case NR33:
		{
			ch3Freq = (ch3Freq&0xF00) + val;
		}
		break;

		case NR34:
		{
			ch3Restart = (val&0x80)?1:0;
			ch3Counter = (val&0x40)?1:0;
			ch3Freq = (ch3Freq&0xFF) + ((val&0x7)<<8);
		}
		break;

		case NR41:
		{
			ch4SoundLength = (val&0x3F);
		}
		break;

		case NR42:
		{
			ch4VolumeEnvelope = (val&0xF0)>>4;
			ch4EnvelopeChange = (val&0x08)?1:0;
			ch4EnvelopeSweep = (val&0x07);
		}
		break;

		case NR43:
		{
			ch4ShiftClock = (val&0xF0)>>4;
			ch4CounterWidth = (val&0x08)?1:0;
			ch4DivRatio = (val&0x07);
		}
		break;

		case NR44:
		{
			ch4Restart = (val&0x80)?1:0;
			ch4Counter = (val&0x40)?1:0;
		}
		break;

		case NR50:
		{
			enableVINtoSO2 = (val&0x80)?1:0;
			SO2outLevel = (val&0x70)>>4;
			enableVINtoSO1 = (val&0x08)?1:0;
			SO1outLevel = (val&0x07);
		}
		break;

		case NR51:
		{
			ch4toSO2 = (val&0x80)?1:0;
			ch3toSO2 = (val&0x40)?1:0;
			ch2toSO2 = (val&0x20)?1:0;
			ch1toSO2 = (val&0x10)?1:0;
			ch4toSO1 = (val&0x08)?1:0;
			ch3toSO1 = (val&0x04)?1:0;
			ch2toSO1 = (val&0x02)?1:0;
			ch1toSO1 = (val&0x01)?1:0;
		}
		break;

		case NR52:
		{
			soundON = (val&0x80)?1:0;
		}
		break;	
	}
}

void AUDIO::tick(int cycle)
{

}

void AUDIO::updateStream(void* userdata, Uint8* stream, int len)
{
	if(len == 0){return;}

}

void AUDIO::close()
{
	SDL_PauseAudioDevice(device, 1);
	SDL_CloseAudioDevice(device);
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
