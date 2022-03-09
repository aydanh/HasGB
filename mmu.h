#ifndef _MMU_H
#define _MMU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enum.h"
#include "gpu.h"
#include "key.h"
#include "timer.h"
#include "audio.h"

#ifndef BYTE_TYPE_DEFS
#define BYTE_TYPE_DEFS
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
#endif

#ifndef MEMORY_DEFS
#define MEMORY_SIZE_DEFS
#define ROM_SIZE 0x4000
#define RAM_SIZE 0x1000
#define HRAM_SIZE 0x100
#endif

typedef struct
{
	u8 title[16];
	u8 sgbFlag;
	u8 cartType;
	u8 romSize;
	u8 ramSize;
	u8 regionCode;
	//u8 licenceCode[2];
	//u8 romVersion;
}gameSpecs;

typedef struct
{
	u8 seconds;
	u8 minutes;
	u8 hours;
	u8 lday;
	u8 hdayc;
}RTC;

class MMU
{
private:
	u8 rom0[ROM_SIZE];
	u8** rom1;
	u8 wram[8][RAM_SIZE];
	u8 hram[HRAM_SIZE];
	u8** eram;

	int wramSelect;

	int cartridge;
	int ramPresent;
	int battPresent;
	int rumblePresent;
	int timePresent;
	RTC rtc;

	int ramEnable;
	int ramMode;
	u8 selectl;
	u8 selecth;
	u8 selectr;

	int ramBankCount;
	int romBankCount;
	int currentRomBank;
	int currentRamBank;

	char* savepath;

	GPU* gpu;
	KEY* key;
	TIMER* timer;
	AUDIO* audio;

	u16 hdmaSource;
	u16 hdmaDest;
	int hdmaLength;
	int hdmaMode;
	int hdmaActive;	

	u8 unDoc6C;
	u8 unDoc72;
	u8 unDoc73;
	u8 unDoc74;
	u8 unDoc75;
	u8 unDoc76;
	u8 unDoc77;

public:
	u8* intrFlag;
	u8* intrMask;
	int gbMode;
	int speedMode;
	int speedSwitch;

	int dmaDone;

	gameSpecs game;

public:
	MMU();
	~MMU();

	void init();

	void reset();

	void setGame(char* buffer, const char* gamepath);

	u8 read(u16 addr);
	u16 readB(u16 addr);

	void write(u16 addr, u8 val);
	void writeB(u16 addr, u16 val);

	void dma(u16 addr);
	void hdma();
	int getHDMAstate();

	void bindPeripheral(GPU* n_gpu, TIMER* n_time, KEY* n_key, AUDIO* n_audio);

	void chipControl(u16 addr, u8 val);
	void bankSelect();
	u8 readExRam(u16 addr);
	void writeExRam(u16 addr, u8 val);

	void close();
};

#endif	