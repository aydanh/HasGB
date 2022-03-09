#ifndef _GPU_H
#define _GPU_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "enum.h"

#ifndef BYTE_TYPE_DEFS
#define BYTE_TYPE_DEFS
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
#endif

#ifndef VIDEO_DEFS
#define VIDEO_DEFS
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 288
#define TILE_DATA_SIZE 384
#define VRAM_SIZE 0x2000
#define OAM_SIZE 0xA0
#define COLOR_RATIO 255/31
#endif

typedef struct
{
	int y,x;
	u8 tileno;
	int attr[6];
}Sprite;

typedef struct
{
	int palNo;
	int bankNo;
	int hFlip;
	int vFlip;
	int prio;
}BgAttr;

class GPU
{
private:
	SDL_Surface* buffer;
	SDL_Surface* big;
	SDL_Surface* lcd;
	SDL_Window* window;

	int lcd_cycle;
	int tileData[2][TILE_DATA_SIZE][8][8];
	Sprite sprite[40];
	int screenInfo[144][160];
	BgAttr bgInfo[2][1024];
	int lineSpriteCount;

	int vramSelect;
	u8 vram[2][VRAM_SIZE];
	u8 oam[OAM_SIZE];
	
	int screenDisplay;
	int bgwDisplay;
	int bgTileMap;
	int windowDisplay;
	int windowTileMap;
	int tileDataLocation;
	int spriteDisplay;
	int spriteSize16;
	int bgwPrio;

	int coincideIntr;
	int mode0Intr;
	int mode1Intr;
	int mode2Intr;
	int coincide;
	int mode;

	int first;
	int pastVblank;
	int pastHblank;
	int pastLine[144];
	int pastHblank8F;


	u8 bgy;
	u8 bgx;
	u8 line;
	u8 coincideLine;
	u8 vBlankLine;
	int vBlankIntrReq;
	int x90IntrReq;

	u8 bgPal;
	u8 objPal0;
	u8 objPal1;

	u8 windowy;
	u8 windowx;

	u16 cBgPal[8][4];
	u8 cBgpIndex;
	int autoIncBgPal;

	u16 cObjPal[8][4];
	u8 cObjpIndex;
	int autoIncObjPal;

	//int renderReady;
public:
	u8* intrFlag;
	int* gbMode;

public:
	GPU();
	~GPU();

	int init();

	void reset();

	int tick(int cycle);
	
	u8 read(u16 addr);
	void write(u16 addr, u8 val);

	void updateTileData(u16 offset, u8 val);
	void updateSpriteData(u16 offset, u8 val);
	void updateBgData(u16 offset, u8 val);
	int scanLine();
	int scanLineColor();
	int scanSprite();
	int scanSpriteColor();
	int render();

	Uint32 getRGB(u16 palData);

	void close();	
};
	
#endif