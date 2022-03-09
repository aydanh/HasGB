#ifndef _HASGB_H
#define _HASGB_H

#include <SDL2/SDL.h>
#include "gpu.h"
#include "audio.h"
#include "key.h"
#include "timer.h"
#include "mmu.h"
#include "cpu.h"
#include <stdlib.h>

class HASGB
{
private:
	GPU gpu;
	KEY key;
	TIMER timer;
	MMU mmu;
	CPU cpu;
	AUDIO audio;
	
	gameSpecs game;
	
	int comm;
	int valid;
	int nextPC;
	int dbg;

public:
	HASGB();
	~HASGB();

	int init();

	void reset();
	int loadGame(const char* gamepath);
	int handleEvent(SDL_Event e);
	void run();
	//void close();
	void printGameSpecs();

	void debugRun();
	u16 getPC();

	void close();
};

#endif