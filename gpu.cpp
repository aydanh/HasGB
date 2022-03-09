#include "gpu.h"

GPU::GPU() {}
GPU::~GPU() {}	

int GPU::init()
{
	if( SDL_InitSubSystem( SDL_INIT_VIDEO ) != 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return 0;
    }
    else
    {
        window = SDL_CreateWindow( "HasGB", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			return 0;
        }
        else
        {
            lcd = SDL_GetWindowSurface( window );			
        }
    }
	
	Uint32 r,g,b,a;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			r = 0xff000000;
			g = 0x00ff0000;
			b = 0x0000ff00;
			a = 0x000000ff;
#else
			a = 0xff000000;
			b = 0x00ff0000;
			g = 0x0000ff00;
			r = 0x000000ff;
#endif
	buffer = SDL_CreateRGBSurface(0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 32,r,g,b,a);
	big = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,r,g,b,a);
	if(!buffer || !big)
	{
		printf("Could not create RGB Surface! SDL_Error %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}

void GPU::reset()
{
	lcd_cycle = 0;
	vramSelect = 0;
	lineSpriteCount = 0;
	
	int i,j,k;
	for(i=0; i<TILE_DATA_SIZE; i++) { for(j=0; j<8; j++) {	for(k=0; k<8; k++)	{	tileData[0][i][j][k] = 0; tileData[1][i][j][k] = 0;	}	}	}
	for(i=0; i<OAM_SIZE; i++) { oam[i] = 0;		vram[0][i] = 0; vram[1][i] = 0; }
	for(i=OAM_SIZE; i<VRAM_SIZE; i++) {			vram[0][i] = 0; vram[1][i] = 0; }

	for(i=0; i<1024; i++)
	{
		bgInfo[0][i].palNo = 0;
		bgInfo[0][i].bankNo = 0;
		bgInfo[0][i].hFlip = 0;
		bgInfo[0][i].vFlip = 0;
		bgInfo[0][i].prio = 0;

		bgInfo[1][i].palNo = 0;
		bgInfo[1][i].bankNo = 0;
		bgInfo[1][i].hFlip = 0;
		bgInfo[1][i].vFlip = 0;
		bgInfo[1][i].prio = 0;
	}

	for(i=0; i<40; i++)
	{
		sprite[i].y = 144;
		sprite[i].x = 160;
		sprite[i].tileno = 0;
		sprite[i].attr[PRIO] = 0;
		sprite[i].attr[YFLIP] = 0;
		sprite[i].attr[XFLIP] = 0;
		sprite[i].attr[PAL] = 0;
		sprite[i].attr[BANKNO] = 0;
		sprite[i].attr[CPAL] = 0;
	}
	
	for(i=0; i<144; i++)
	{
		for(j=0; j<160; j++)
		{
			screenInfo[i][j] = 0;
		}
	}


	screenDisplay = 1;
	bgwDisplay = 1;
	bgTileMap = 0;
	windowDisplay = 0;
	windowTileMap = 0;
	tileDataLocation = 1;
	spriteDisplay = 0;
	spriteSize16 = 0;
	bgwPrio = 1;

	coincideIntr = 0;
	mode0Intr = 0;
	mode1Intr = 0;
	mode2Intr = 0;
	coincide = 0;
	mode = 0;

	first = 0;
	pastHblank = 0;
	pastVblank = 0;
	for(int i=0; i<144; i++){ pastLine[i] = 0; }
	pastHblank8F = 0;

	 bgy = 0;
	 bgx = 0;
	 line = 0;
	 coincideLine = 0;
	 vBlankLine = 0;
	 vBlankIntrReq = 0;
	 x90IntrReq = 0;

	 bgPal = 0xFC;
	 objPal0 = 0xFF;
	 objPal1 = 0xFF;

	 windowy = 0;
	 windowx = 0;

	for(i=0; i<8; i++)
	{
		for(int j=0; j<4; j++)
		{
			cBgPal[i][j] = 0;
			cObjPal[i][j] = 0;
		}
	}

	cBgpIndex = 0;
	autoIncBgPal = 0;

	cObjpIndex = 0;
	autoIncObjPal = 0;
}



void GPU::updateTileData(u16 offset, u8 val) 
{
	int x;
	int nth = offset>>4;
	int y = offset&15;
	if(y&0x1){
		for(x=0; x<8; x++) {
			tileData[vramSelect][nth][y/2][x] = ((vram[vramSelect][offset]>>(7-x))&0x1)?2:0;
			tileData[vramSelect][nth][y/2][x] += ((vram[vramSelect][offset-1]>>(7-x))&0x1)?1:0;
		}
	}else{
		for(x=0; x<8; x++) {
			tileData[vramSelect][nth][y/2][x] = ((vram[vramSelect][offset]>>(7-x))&0x1)?1:0;
			tileData[vramSelect][nth][y/2][x] += ((vram[vramSelect][offset+1]>>(7-x))&0x1)?2:0;
		}
	}
}

void GPU::updateSpriteData(u16 offset, u8 val)
{	
	int i = offset>>2;
	switch(offset&0x3)
	{
		case 0x0:
			sprite[i].y = val-16;
		break;
			
		case 0x1:
			sprite[i].x = val-8;
		break;
			
		case 0x2:
			sprite[i].tileno = val;
		break;
			
		case 0x3:
			sprite[i].attr[PRIO] = (val&0x80)?1:0;
			sprite[i].attr[YFLIP] = (val&0x40)?1:0;
			sprite[i].attr[XFLIP] = (val&0x20)?1:0;
			sprite[i].attr[PAL] = (val&0x10)?1:0;
			sprite[i].attr[BANKNO] = (val&0x08)?1:0;
			sprite[i].attr[CPAL] = (val&0x7);
		break;
	}
}

void GPU::updateBgData(u16 offset, u8 val)
{
	if(offset<0x1C00)
	{
		int n = offset-0x1800;
		bgInfo[0][n].palNo = (val&0x7);
		bgInfo[0][n].bankNo = (val&0x8)?1:0;
		bgInfo[0][n].hFlip = (val&0x20)?1:0;
		bgInfo[0][n].vFlip = (val&0x40)?1:0;
		bgInfo[0][n].prio = (val&0x80)?1:0;
	}
	else
	{
		int n = offset-0x1C00;
		bgInfo[1][n].palNo = (val&0x7);
		bgInfo[1][n].bankNo = (val&0x8)?1:0;
		bgInfo[1][n].hFlip = (val&0x20)?1:0;
		bgInfo[1][n].vFlip = (val&0x40)?1:0;
		bgInfo[1][n].prio = (val&0x80)?1:0;
	}
}

int GPU::tick(int cycle)
{
	if(screenDisplay) {
		lcd_cycle += cycle;
		switch(mode) 
		{
			case 0:			//hblank
			{
				//if(first) { first = 0; return 2; }
				if(lcd_cycle >= 204) {
					lcd_cycle -= 204;
					line++;
					if(line == coincideLine) 
					{
						coincide = 1;
						if(coincideIntr){ 
							if(line != 0x90){ 
								(*intrFlag) |= 0x2; pastLine[line] = 1;
							}else{
								x90IntrReq = 1;
								(*intrFlag) |= 0x2;
							}
						}
					}else{coincide = 0;}
					
					if(line > 143) {
						mode = 1; first = 1;
						//if(!pastHblank && !pastLine[0x8F]){ 
							if(mode1Intr){ (*intrFlag) |= 0x02; pastVblank = 1; } 
						//	if(mode2Intr){ (*intrFlag) |= 0x02; }
						//}
						(*intrFlag) |= 0x01;
						vBlankIntrReq = 1;

						if(!render()) { return 0; }
					}else{
						mode = 2;
						if(!pastHblank && !pastLine[line-1]){ 
							if(mode2Intr){ (*intrFlag) |= 0x02; } 
						}
					}
					pastHblank=0;
					return 1 ;
				}
			}
			break;
			case 1:			//vblank
			{
				/*if(x90IntrReq)
				{
					(*intrFlag) |= 0x2;
					x90IntrReq = 0;
				}
				if(vBlankIntrReq)
				{
					//if(!pastHblank8F && !pastLine[0x8F]){ 
						if(mode1Intr){ (*intrFlag) |= 0x02; pastVblank = 1; } 
						if(mode2Intr){ (*intrFlag) |= 0x02; }
					//}
					(*intrFlag) |= 0x01;
					vBlankIntrReq = 0;
					pastHblank8F = 0;
				}*/
				if(vBlankLine==9 && first && lcd_cycle>=56)
				{
					for(int i=0; i<144; i++){ pastLine[i] = 0; }
					line = 0;
					if(coincideLine == 0) { 
						coincide = 1;
						//if(!pastVblank){ 
							if(coincideIntr){ (*intrFlag) |= 0x2; pastLine[0] = 1; }
						//}
					}else{coincide = 0;}
					first = 0;
				}
				if(lcd_cycle >= 456) {
					lcd_cycle -= 456;
					vBlankLine++;
					line++;
					
					if(vBlankLine > 9) 
					{
						vBlankLine = 0;
						line=0;

						mode = 2;
						//if(!pastVblank && !pastLine[0]){ 
							if(mode2Intr){ (*intrFlag) |= 0x02; }	
						//}

						pastVblank = 0;
						return 1;
					}

					if(line == coincideLine) 
					{
						coincide = 1;
						//if(!pastVblank){ 
							if(coincideIntr){ (*intrFlag) |= 0x2; } 
						//}
					}else{coincide = 0;}

					return 1;
				}
			}		
			break;
			case 2:			//oam
			{
				if(lcd_cycle >= 80) {
					lcd_cycle -= 80;
					mode = 3;
					return 1;
				}
			}
			break;
			case 3:			//vram + oam
			{
				if(lcd_cycle >= 172) {
					lcd_cycle -= 172;
					mode = 0;
					//if(!pastLine[line-1]){ 
						if(mode0Intr){ (*intrFlag) |= 0x02; pastHblank = 1; if(line == 0x8F) { pastHblank8F = 1; }} 
					//}
					//first = 1;
					
					if((*gbMode) == NON_GBC_MODE)
					{
						if(!scanLine()) { return 0; }
						if(spriteDisplay)
						{
							if(!scanSprite()) { return 0; }
						}
					}
					else
					{
						if(!scanLineColor()) { return 0; }
						if(spriteDisplay)
						{
							if(!scanSpriteColor()) { return 0; }
						}
					}
					return 2;
				}
			}
			break;
		}
	}
	return 1;
}

int GPU::scanLine()
{
	if(SDL_LockSurface(buffer) < 0)
	{
		printf("Could not lock surface while scanline! SDL_Error %s\n", SDL_GetError());
		return 0;
	}
	Uint32* pixels = (Uint32*)buffer->pixels;

	Uint32 black = SDL_MapRGBA(buffer->format, 0x20, 0x18, 0x08, 255);
	Uint32 dgrey = SDL_MapRGBA(buffer->format, 0x38, 0x58, 0x20, 255);
	Uint32 lgrey = SDL_MapRGBA(buffer->format, 0x50, 0xA8, 0x70, 255);
	Uint32 white = SDL_MapRGBA(buffer->format, 0x90, 0xD8, 0xB8, 255);
	Uint32 colors[4] = { white, lgrey, dgrey, black };
	
	if(bgwDisplay)
	{
		u16 tilemap = (bgTileMap)?0x1C00:0x1800;
	
		int tilerow = ((bgy + line)%256)/8;
		int tilecol = (bgx)/8;

		u8 tileoff = (vram[0][tilemap + tilerow*32 + tilecol]);
		int tileno = (tileDataLocation)?tileoff:(256 + (signed char)tileoff);

		int y = (bgy + line)%8;
		int x = (bgx)%8;
	
	
		int i;
		for(i=0; i<160; i++) 
		{
			pixels[line*160 + i] = colors[((bgPal)>>(2*(tileData[0][tileno][y][x])))&0x3];
			if(tileData[0][tileno][y][x] != 0)
			{
				screenInfo[line][i] = 1;
			}
			else
			{
				screenInfo[line][i] = 0;
			}
			x++;
			if(x==8) 
			{
				x = 0;
				tilecol = (tilecol+1)%32;
				tileoff = vram[0][tilemap + tilerow*32 + tilecol];
				tileno = (tileDataLocation)?tileoff:(256 + (signed char)tileoff);
			}
		}
	}
	else
	{
		SDL_FillRect(buffer,NULL,SDL_MapRGB(buffer->format, 255,255,255)); 
	}	
	
	if(windowDisplay && windowx<=166 && windowy <= line)
	{
		u16 tilemap = (windowTileMap)?0x1C00:0x1800;
		
		int tilerow = (line - windowy)/8;
		int tilecol = 0;		

		u8 tileoff = vram[0][tilemap + tilerow*32 + tilecol];
		int tileno = (tileDataLocation)?tileoff:(256 + (signed char)tileoff);

		int y = (line - windowy)%8;
		int x = 0;

		int col = windowx - 7;
		int a = (col<0)?-col:0;
		col = (col>0)?(col):0;

		int i;
		for(i=col; i<160; i++) 
		{
			pixels[line*160 + i] = colors[((bgPal)>>(2*(tileData[0][tileno][y][x+a])))&0x3];
			if(tileData[0][tileno][y][x+a] != 0)
			{
				screenInfo[line][i] = 1;
			}
			else
			{
				screenInfo[line][i] = 0;
			}
			x++;
			if(x+a==8) 
			{
				x = 0;
				a = 0;
				tilecol = (tilecol+1)%32;
				tileoff = vram[0][tilemap + tilerow*32 + tilecol];
				tileno = (tileDataLocation)?tileoff:(256 + (signed char)tileoff);
			}
		}
	}
	
	SDL_UnlockSurface(buffer);
	return 1;
}

int GPU::scanLineColor()
{
	if(SDL_LockSurface(buffer) < 0)
	{
		printf("Could not lock surface while scanline! SDL_Error %s\n", SDL_GetError());
		return 0;
	}

	Uint32* pixels = (Uint32*)buffer->pixels;

	u16 tilemap = (bgTileMap)?0x1C00:0x1800;
	
	int tilerow = ((bgy + line)%256)/8;
	int tilecol = (bgx)/8;

	int palNo = bgInfo[bgTileMap][tilerow*32 + tilecol].palNo;
	int bankNo = bgInfo[bgTileMap][tilerow*32 + tilecol].bankNo;
	int hFlip = bgInfo[bgTileMap][tilerow*32 + tilecol].hFlip;
	int vFlip = bgInfo[bgTileMap][tilerow*32 + tilecol].vFlip;
	int prio = bgInfo[bgTileMap][tilerow*32 + tilecol].prio;
	
	u16* palData = cBgPal[palNo];
	Uint32 color0 = getRGB(palData[0]);
	Uint32 color1 = getRGB(palData[1]);
	Uint32 color2 = getRGB(palData[2]);
	Uint32 color3 = getRGB(palData[3]);
	Uint32* colors[4] = { &color0, &color1, &color2, &color3 };

	u8 tileoff = vram[0][tilemap + tilerow*32 + tilecol];
	int tileno = (tileDataLocation)?tileoff:(256 + (signed char)tileoff);

	int y = (bgy + line)%8;
	if(vFlip){ y = 7-y; }
	int x = (bgx)%8;
	if(hFlip){ x = 7-x; }

	int i;
	for(i=0; i<160; i++) 
	{
		if(!hFlip)
		{
			pixels[line*160 + i] = (*colors[tileData[bankNo][tileno][y][x]]);
			if(bgwPrio)
			{
				if(!prio)
				{
					if(tileData[bankNo][tileno][y][x] != 0)
					{
						screenInfo[line][i] = 1;
					}
					else
					{
						screenInfo[line][i] = 0;
					}
				}
				else
				{ 
					if(tileData[bankNo][tileno][y][x] != 0)
					{
						screenInfo[line][i] = 2;
					}
					else
					{
						screenInfo[line][i] = 0;
					}
				
				}
			}
			else
			{
				screenInfo[line][i] = 0;
			}
			x++;
			if(x>7) 
			{
				tilecol = (tilecol+1)%32;
				//tilemap = (bgTileMap)?0x1C00:0x1800;

				palNo = bgInfo[bgTileMap][tilerow*32 + tilecol].palNo;
				bankNo = bgInfo[bgTileMap][tilerow*32 + tilecol].bankNo;
				hFlip = bgInfo[bgTileMap][tilerow*32 + tilecol].hFlip;
				vFlip = bgInfo[bgTileMap][tilerow*32 + tilecol].vFlip;
				prio = bgInfo[bgTileMap][tilerow*32 + tilecol].prio;

				tileoff = vram[0][tilemap + tilerow*32 + tilecol];
				tileno = (tileDataLocation)?tileoff:(256 + (signed char)tileoff);

				palData = cBgPal[palNo];
				color0 = getRGB(palData[0]);
				color1 = getRGB(palData[1]);
				color2 = getRGB(palData[2]);
				color3 = getRGB(palData[3]);

				if(hFlip){ x = 7; }
				else{ x = 0; }
				y = (bgy+ line)%8;
				if(vFlip){ y = 7-y; }
			}
		}
		else
		{
			pixels[line*160 + i] = (*colors[tileData[bankNo][tileno][y][x]]);
			if(bgwPrio)
			{
				if(!prio)
				{
					if(tileData[bankNo][tileno][y][x] != 0)
					{
						screenInfo[line][i] = 1;
					}
					else
					{
						screenInfo[line][i] = 0;
					}
				}
				else
				{ 
					if(tileData[bankNo][tileno][y][x] != 0)
					{
						screenInfo[line][i] = 2;
					}
					else
					{
						screenInfo[line][i] = 0;
					}
				}
			}
			else
			{
				screenInfo[line][i] = 0;
			}
			x--;
			if(x<0) 
			{
				tilecol = (tilecol+1)%32;
				//tilemap = (bgTileMap)?0x1C00:0x1800;

				palNo = bgInfo[bgTileMap][tilerow*32 + tilecol].palNo;
				bankNo = bgInfo[bgTileMap][tilerow*32 + tilecol].bankNo;
				hFlip = bgInfo[bgTileMap][tilerow*32 + tilecol].hFlip;
				vFlip = bgInfo[bgTileMap][tilerow*32 + tilecol].vFlip;
				prio = bgInfo[bgTileMap][tilerow*32 + tilecol].prio;

				tileoff = vram[0][tilemap + tilerow*32 + tilecol];
				tileno = (tileDataLocation)?tileoff:(256 + (signed char)tileoff);

				palData = cBgPal[palNo];
				color0 = getRGB(palData[0]);
				color1 = getRGB(palData[1]);
				color2 = getRGB(palData[2]);
				color3 = getRGB(palData[3]);

				if(hFlip){ x = 7; }
				else{ x = 0; }
				y = (bgy + line)%8;
				if(vFlip){ y = 7-y; }
			}
		}
	}

	
	if(windowDisplay && windowx<=166 && windowy <= line)
	{
		tilemap = (windowTileMap)?0x1C00:0x1800;
		
		tilerow = (line - windowy)/8;
		tilecol = 0;		

		palNo = bgInfo[windowTileMap][tilerow*32 + tilecol].palNo;
		bankNo = bgInfo[windowTileMap][tilerow*32 + tilecol].bankNo;
		hFlip = bgInfo[windowTileMap][tilerow*32 + tilecol].hFlip;
		vFlip = bgInfo[windowTileMap][tilerow*32 + tilecol].vFlip;
		prio = bgInfo[windowTileMap][tilerow*32 + tilecol].prio;

		palData = cBgPal[palNo];
		color0 = getRGB(palData[0]);
		color1 = getRGB(palData[1]);
		color2 = getRGB(palData[2]);
		color3 = getRGB(palData[3]);

		tileoff = vram[0][tilemap + tilerow*32 + tilecol];
		tileno = (tileDataLocation)?tileoff:(256 + (signed char)tileoff);

		y = (line - windowy)%8;
		if(vFlip){ y = 7-y; }
		x = 0;
		if(hFlip){ x = 7; }

		int col = windowx - 7;
		int a = (col<0)?-col:0;
		col = (col>0)?(col):0;

			
		for(i=col; i<160; i++) 
		{
			if(!hFlip)
			{
				pixels[line*160 + i] = (*colors[tileData[bankNo][tileno][y][x+a]]);
				if(bgwPrio)
				{
					if(!prio)
					{
						if(tileData[bankNo][tileno][y][x+a] != 0)
						{
							screenInfo[line][i] = 1;
						}
						else
						{
							screenInfo[line][i] = 0;
						}
					}
					else
					{ 
						if(tileData[bankNo][tileno][y][x+a] != 0)
						{
							screenInfo[line][i] = 2;
						}
						else
						{
							screenInfo[line][i] = 0;
						}
					}
				}
				else
				{
					screenInfo[line][i] = 0;
				}
				x++;
				if(x+a>7) 
				{
					a = 0;
					tilecol = (tilecol+1)%32;
					//tilemap = (windowTileMap)?0x1C00:0x1800;
					
					palNo = bgInfo[windowTileMap][tilerow*32 + tilecol].palNo;
					bankNo = bgInfo[windowTileMap][tilerow*32 + tilecol].bankNo;
					hFlip = bgInfo[windowTileMap][tilerow*32 + tilecol].hFlip;
					vFlip = bgInfo[windowTileMap][tilerow*32 + tilecol].vFlip;
					prio = bgInfo[windowTileMap][tilerow*32 + tilecol].prio;

					tileoff = vram[0][tilemap + tilerow*32 + tilecol];
					tileno = (tileDataLocation)?tileoff:(256 + (signed char)tileoff);

					palData = cBgPal[palNo];
					color0 = getRGB(palData[0]);
					color1 = getRGB(palData[1]);
					color2 = getRGB(palData[2]);
					color3 = getRGB(palData[3]);

					if(hFlip){ x = 7; }
					else{ x = 0; }
					y = (line - windowy)%8;
					if(vFlip){ y = 7-y; }
				}
			
			}
			else
			{
				pixels[line*160 + i] = (*colors[tileData[bankNo][tileno][y][x-a]]);
				if(bgwPrio)
				{
					if(!prio)
					{
						if(tileData[bankNo][tileno][y][x-a] != 0)
						{
							screenInfo[line][i] = 1;
						}
						else
						{
							screenInfo[line][i] = 0;
						}
					}
					else
					{ 
						if(tileData[bankNo][tileno][y][x-a] != 0)
						{
							screenInfo[line][i] = 2;
						}
						else
						{
							screenInfo[line][i] = 0;
						}
					}
				}
				else
				{
					screenInfo[line][i] = 0;
				}
				x--;
				if(x-a<0) 
				{
					a = 0;
					tilecol = (tilecol+1)%32;
					//tilemap = (windowTileMap)?0x1C00:0x1800;

					palNo = bgInfo[windowTileMap][tilerow*32 + tilecol].palNo;
					bankNo = bgInfo[windowTileMap][tilerow*32 + tilecol].bankNo;
					hFlip = bgInfo[windowTileMap][tilerow*32 + tilecol].hFlip;
					vFlip = bgInfo[windowTileMap][tilerow*32 + tilecol].vFlip;
					prio = bgInfo[windowTileMap][tilerow*32 + tilecol].prio;

					tileoff = vram[0][tilemap + tilerow*32 + tilecol];
					tileno = (tileDataLocation)?tileoff:(256 + (signed char)tileoff);

					palData = cBgPal[palNo];
					color0 = getRGB(palData[0]);
					color1 = getRGB(palData[1]);
					color2 = getRGB(palData[2]);
					color3 = getRGB(palData[3]);

					if(hFlip){ x = 7; }
					else{ x = 0; }
					y = (line - windowy)%8;
					if(vFlip){ y = 7-y; }
				}
			}
		}
	}

	SDL_UnlockSurface(buffer);
	return 1;
}

int GPU::scanSprite()
{
	if(SDL_LockSurface(buffer) < 0)
	{
		printf("Could not lock surface! SDL_Error %s\n", SDL_GetError());
		return 0;
	}
	Uint32* pixels = (Uint32*)buffer->pixels;

	Uint32 black = SDL_MapRGBA(buffer->format, 0x20, 0x18, 0x08, 255);
	Uint32 dgrey = SDL_MapRGBA(buffer->format, 0x38, 0x58, 0x20, 255);
	Uint32 lgrey = SDL_MapRGBA(buffer->format, 0x50, 0xA8, 0x70, 255);
	Uint32 white = SDL_MapRGBA(buffer->format, 0x90, 0xD8, 0xB8, 255);
	Uint32 colors[4] = { white, lgrey, dgrey, black };	
	

	int notUsed[40];
	int i;
	
	for(i=0; i<40; i++) { notUsed[i] = 0; }

	
	for(i=0; i<40; i++) {
		if(!spriteSize16){
			if((sprite[i].y <= line) && (sprite[i].y > line-8))
			{
				if((sprite[i].x <= 160) && (sprite[i].x > -8))
				{
					notUsed[i] = 1;
				}
			}
		}else{
			if((sprite[i].y <=line) && (sprite[i].y > line-16))
			{
				if((sprite[i].x <= 160) && (sprite[i].x > -8))
				{
					notUsed[i] = 1;
				}
			}
		}
	}
	
	lineSpriteCount = 0;
	while(lineSpriteCount<10)
	{
		int n = 40;
		for(i=0; i<40; i++) {
			if(notUsed[i]){
				if(n==40) { n=i; }
				else{
					if(sprite[i].x < sprite[n].x) { n=i; }
				}
			}
		}
		if(n==40){ break; }
		
		notUsed[n] = 0;
		
		u8 objp = (sprite[n].attr[PAL])?objPal1:objPal0;
		
		int y = line -  sprite[n].y;
		if(sprite[n].attr[YFLIP]){ 
			if(!spriteSize16) { y = 7-y; }
			else{ y = 15-y; }
		}
		int x = sprite[n].x;
		
		u8 tileno = (spriteSize16)?((y<8)?(sprite[n].tileno&0xFE):(sprite[n].tileno|0x01)):sprite[n].tileno;
		y = (y<8)?y:(y-8);


		if(x<0)
		{
			int a = -x;
			if(!sprite[n].attr[XFLIP])
			{
				for(int j=a; j<8; j++)
				{
					if(tileData[0][tileno][y][j] > 0)
					{
						if(!sprite[n].attr[PRIO])
						{
							if(screenInfo[line][j-a] < 2)
							{
								pixels[line*160 + j-a] = colors[(objp>>(2*tileData[0][tileno][y][j]))&0x3];
								screenInfo[line][j-a] = 2;
							}
						}
						else
						{
							if(screenInfo[line][j-a] == 0)
							{
								pixels[line*160 + j-a] = colors[(objp>>(2*tileData[0][tileno][y][j]))&0x3];
								screenInfo[line][j-a] = 2;
							}
						}
					}
				}
			}
			else
			{
				for(int j=7-a; j>=0; j--)
				{
					if(tileData[0][tileno][y][j] > 0)
					{
						if(!sprite[n].attr[PRIO])
						{
							if(screenInfo[line][7-a-j] < 2)
							{
								pixels[line*160 + 7-a-j] = colors[(objp>>(2*tileData[0][tileno][y][j]))&0x3];
								screenInfo[line][7-a-j] = 2;
							}
						}
						else
						{
							if(screenInfo[line][7-a-j] == 0)
							{
								pixels[line*160 + 7-a-j] = colors[(objp>>(2*tileData[0][tileno][y][j]))&0x3];
								screenInfo[line][7-a-j] = 2;
							}
						}
					}
				}
			}
		}
		else
		{
			if(!sprite[n].attr[XFLIP])
			{
				for(int j=0; ((j<8)&&((x+j)<160)); j++)
				{
					if(tileData[0][tileno][y][j] > 0)
					{
						if(!sprite[n].attr[PRIO])
						{
							if(screenInfo[line][x+j] < 2)
							{
								pixels[line*160 + x+j] = colors[(objp>>(2*tileData[0][tileno][y][j]))&0x3];
								screenInfo[line][x+j] = 2;
							}
						}
						else
						{
							if(screenInfo[line][x+j] == 0)
							{
								pixels[line*160 + x+j] = colors[(objp>>(2*tileData[0][tileno][y][j]))&0x3];
								screenInfo[line][x+j] = 2;
							}
						}
					}
				}
			}
			else
			{
				for(int j=7; ((j>=0)&&((x+7-j)<160)); j--)
				{
					if(tileData[0][tileno][y][j] > 0)
					{
						if(!sprite[n].attr[PRIO])
						{
							if(screenInfo[line][x+7-j] < 2)
							{
								pixels[line*160 + x+7-j] = colors[(objp>>(2*tileData[0][tileno][y][j]))&0x3];
								screenInfo[line][x+7-j] = 2;
							}
						}
						else
						{
							if(screenInfo[line][x+7-j] == 0)
							{
								pixels[line*160 + x+7-j] = colors[(objp>>(2*tileData[0][tileno][y][j]))&0x3];
								screenInfo[line][x+7-j] = 2;
							}
						}
					}
				}
			}
		}
		lineSpriteCount++;
	}		
		
	SDL_UnlockSurface(buffer);
	return 1;
}

int GPU::scanSpriteColor()
{
	if(SDL_LockSurface(buffer) < 0)
	{
		printf("Could not lock surface! SDL_Error %s\n", SDL_GetError());
		return 0;
	}

	Uint32* pixels = (Uint32*)buffer->pixels;
	
	int i,s;
	lineSpriteCount = 0;
	for(i=0; ((i<40)&&(lineSpriteCount<10)); i++) {
		s = (spriteSize16)?16:8;
		if((sprite[i].y <= line) && (sprite[i].y > line-s))
		{
			if((sprite[i].x <= 160) && (sprite[i].x > -8))
			{
				int objp = (sprite[i].attr[CPAL]);
				u16* palData = cObjPal[objp];
				Uint32 color0 = getRGB(palData[0]);
				Uint32 color1 = getRGB(palData[1]);
				Uint32 color2 = getRGB(palData[2]);
				Uint32 color3 = getRGB(palData[3]);
				Uint32 colors[4] = { color0, color1, color2, color3 };

				int bankNo = sprite[i].attr[BANKNO];
		
				int y = line -  sprite[i].y;
				if(sprite[i].attr[YFLIP]){ 
					if(!spriteSize16) { y = 7-y; }
					else{ y = 15-y; }
				}
				int x = sprite[i].x;
		
				u8 tileno = (spriteSize16)?((y<8)?(sprite[i].tileno&0xFE):(sprite[i].tileno|0x01)):sprite[i].tileno;
				y = (y<8)?y:(y-8);

				if(x<0)
				{
					int a = -x;
					if(!sprite[i].attr[XFLIP])
					{
						for(int j=a; j<8; j++)
						{
							if(tileData[bankNo][tileno][y][j] != 0)
							{
								if(!sprite[i].attr[PRIO])
								{
									if(screenInfo[line][j-a] < 2)
									{
										pixels[line*160 + j-a] = colors[tileData[bankNo][tileno][y][j]];
										screenInfo[line][j-a] = 2;
									}
								}
								else
								{
									if(screenInfo[line][j-a] == 0)
									{
										pixels[line*160 + j-a] = colors[tileData[bankNo][tileno][y][j]];
										screenInfo[line][j-a] = 2;
									}
								}
							}
						}
					}
					else
					{
						for(int j=7-a; j>=0; j--)
						{
							if(tileData[bankNo][tileno][y][j] != 0)
							{
								if(!sprite[i].attr[PRIO])
								{
									if(screenInfo[line][7-a-j] < 2)
									{
										pixels[line*160 + 7-a-j] = colors[tileData[bankNo][tileno][y][j]];
										screenInfo[line][7-a-j] = 2;
									}
								}
								else
								{
									if(screenInfo[line][7-a-j] == 0)
									{
										pixels[line*160 + 7-a-j] = colors[tileData[bankNo][tileno][y][j]];
										screenInfo[line][7-a-j] = 2;
									}
								}
							}
						}
					}
				}
				else
				{
					if(!sprite[i].attr[XFLIP])
					{
						for(int j=0; ((j<8)&&((x+j)<160)); j++)
						{
							if(tileData[bankNo][tileno][y][j] != 0)
							{
								if(!sprite[i].attr[PRIO])
								{
									if(screenInfo[line][x+j] < 2)
									{
										pixels[line*160 + x+j] = colors[tileData[bankNo][tileno][y][j]];
										screenInfo[line][x+j] = 2;
									}
								}
								else
								{
									if(screenInfo[line][x+j] == 0)
									{
										pixels[line*160 + x+j] = colors[tileData[bankNo][tileno][y][j]];
										screenInfo[line][x+j] = 2;
									}
								}
							}
						}
					}
					else
					{
						for(int j=7; ((j>=0)&&((x+7-j)<160)); j--)
						{
							if(tileData[bankNo][tileno][y][j] != 0)
							{
								if(!sprite[i].attr[PRIO])
								{
									if(screenInfo[line][x+7-j] < 2)
									{
										pixels[line*160 + x+7-j] = colors[tileData[bankNo][tileno][y][j]];
										screenInfo[line][x+7-j] = 2;
									}
								}
								else
								{
									if(screenInfo[line][x+7-j] == 0)
									{
										pixels[line*160 + x+7-j] = colors[tileData[bankNo][tileno][y][j]];
										screenInfo[line][x+7-j] = 2;
									}
								}
							}
						}
					}
				}
				lineSpriteCount++;
			}
		}
	}
		
	SDL_UnlockSurface(buffer);
	return 1;
}

int GPU::render()
{
	if((SDL_LockSurface(buffer)<0) || (SDL_LockSurface(big)<0))
	{
		printf("Could not lock surfaces while render! SDL_Error %s\n", SDL_GetError());
		return 0;
	}

	Uint32* pixels = (Uint32*)buffer->pixels;
	Uint32* bigp = (Uint32*)big->pixels;
	
	int i,j;
	for(i=0; i<SCREEN_HEIGHT/2; i++)
	{
		for(j=0; j<SCREEN_WIDTH/2; j++) 
		{
			Uint32 pix = pixels[i*(SCREEN_WIDTH/2) + j];
			bigp[2*i*SCREEN_WIDTH + 2*j] = pix;
			bigp[(2*i+1)*SCREEN_WIDTH + 2*j] = pix;
			bigp[2*i*SCREEN_WIDTH + 2*j + 1] = pix;
			bigp[(2*i+1)*SCREEN_WIDTH + 2*j + 1] = pix;			
		}
	}
	
	SDL_UnlockSurface(buffer);
	SDL_UnlockSurface(big);
	
	
	
	if(SDL_BlitSurface( big, NULL, lcd, NULL ) < 0)
	{
		printf("Could not blit! SDL_Error %s\n", SDL_GetError());
		return 0;
	}

	if(SDL_UpdateWindowSurface( window )<0)
	{
		printf("Could not update window! SDL_Error %s\n", SDL_GetError());
		return 0;
	}


	return 1;
}

u8 GPU::read(u16 addr)
{
	switch(addr>>12)
	{
		case 8: case 9:
		{
			return vram[vramSelect][addr & 0x1FFF]; 
		}
		break;

		case 15:
		{		
			switch((addr&0x0F00)>>8) 
			{				
			case 14:
				{
						return oam[addr & 0xFF];
				}
				break;
				
				case 15:
				{
					switch(addr&0xFF)
					{
						case LCDC: 
						{
							u8 n = 0;
							n |= (screenDisplay)?0x80:0;
							n |= (windowTileMap)?0x40:0;
							n |= (windowDisplay)?0x20:0;
							n |= (tileDataLocation)?0x10:0;
							n |= (bgTileMap)?0x08:0;
							n |= (spriteSize16)?0x04:0;
							n |= (spriteDisplay)?0x02:0;
							if((*gbMode) == NON_GBC_MODE){ n |= (bgwDisplay)?0x01:0; }
							else{ n |= (bgwPrio)?0x01:0; }
							return n;
						} 
						break;

						case STAT: 
						{
							u8 n = 0x80;
							n |= (coincideIntr)?0x40:0;
							n |= (mode2Intr)?0x20:0;
							n |= (mode1Intr)?0x10:0;
							n |= (mode0Intr)?0x08:0;
							n |= (coincide)?0x04:0;
							if(screenDisplay) {n |= mode; }
							else{ n |= 0x1; }
							return n;
						}
						break;

						case SCY: 
							return bgy;
						break;

						case SCX: 
							return bgx;
						break;

						case LY:
						{
							return line;
						}
						break;
						
						case LYC:
							return coincideLine;
						break;

						case BGP: 
							return bgPal;
						break;
						
						case OBP0: 
							return objPal0;
						break;

						case OBP1: 
							return objPal1;
						break;

						case WY: 
							return windowy;	
						break;

						case WX:
							return windowx;
						break;

						case VBK:
							return vramSelect | 0xFE;
						break;

						case BGPI:
						{
							return (autoIncBgPal<<7) + cBgpIndex + 0x40;
						}
						break;

						case BGPD:
						{
							switch(cBgpIndex)
							{
								case 0: return cBgPal[0][0]&0xFF; break;
								case 1: return cBgPal[0][0]>>8; break;
								case 2: return cBgPal[0][1]&0xFF; break;
								case 3: return cBgPal[0][1]>>8; break;
								case 4: return cBgPal[0][2]&0xFF; break;
								case 5: return cBgPal[0][2]>>8; break;
								case 6: return cBgPal[0][3]&0xFF; break;
								case 7: return cBgPal[0][3]>>8; break;
								case 8: return cBgPal[1][0]&0xFF; break;
								case 9: return cBgPal[1][0]>>8; break;
								case 0xA: return cBgPal[1][1]&0xFF; break;
								case 0xB: return cBgPal[1][1]>>8; break;
								case 0xC: return cBgPal[1][2]&0xFF; break;
								case 0xD: return cBgPal[1][2]>>8; break;
								case 0xE: return cBgPal[1][3]&0xFF; break;
								case 0xF: return cBgPal[1][3]>>8; break;
								case 0x10: return cBgPal[2][0]&0xFF; break;
								case 0x11: return cBgPal[2][0]>>8; break;
								case 0x12: return cBgPal[2][1]&0xFF; break;
								case 0x13: return cBgPal[2][1]>>8; break;
								case 0x14: return cBgPal[2][2]&0xFF; break;
								case 0x15: return cBgPal[2][2]>>8; break;
								case 0x16: return cBgPal[2][3]&0xFF; break;
								case 0x17: return cBgPal[2][3]>>8; break;
								case 0x18: return cBgPal[3][0]&0xFF; break;
								case 0x19: return cBgPal[3][0]>>8; break;
								case 0x1A: return cBgPal[3][1]&0xFF; break;
								case 0x1B: return cBgPal[3][1]>>8; break;
								case 0x1C: return cBgPal[3][2]&0xFF; break;
								case 0x1D: return cBgPal[3][2]>>8; break;
								case 0x1E: return cBgPal[3][3]&0xFF; break;
								case 0x1F: return cBgPal[3][3]>>8; break;
								case 0x20: return cBgPal[4][0]&0xFF; break;
								case 0x21: return cBgPal[4][0]>>8; break;
								case 0x22: return cBgPal[4][1]&0xFF; break;
								case 0x23: return cBgPal[4][1]>>8; break;
								case 0x24: return cBgPal[4][2]&0xFF; break;
								case 0x25: return cBgPal[4][2]>>8; break;
								case 0x26: return cBgPal[4][3]&0xFF; break;
								case 0x27: return cBgPal[4][3]>>8; break;
								case 0x28: return cBgPal[5][0]&0xFF; break;
								case 0x29: return cBgPal[5][0]>>8; break;
								case 0x2A: return cBgPal[5][1]&0xFF; break;
								case 0x2B: return cBgPal[5][1]>>8; break;
								case 0x2C: return cBgPal[5][2]&0xFF; break;
								case 0x2D: return cBgPal[5][2]>>8; break;
								case 0x2E: return cBgPal[5][3]&0xFF; break;
								case 0x2F: return cBgPal[5][3]>>8; break;
								case 0x30: return cBgPal[6][0]&0xFF; break;
								case 0x31: return cBgPal[6][0]>>8; break;
								case 0x32: return cBgPal[6][1]&0xFF; break;
								case 0x33: return cBgPal[6][1]>>8; break;
								case 0x34: return cBgPal[6][2]&0xFF; break;
								case 0x35: return cBgPal[6][2]>>8; break;
								case 0x36: return cBgPal[6][3]&0xFF; break;
								case 0x37: return cBgPal[6][3]>>8; break;
								case 0x38: return cBgPal[7][0]&0xFF; break;
								case 0x39: return cBgPal[7][0]>>8; break;
								case 0x3A: return cBgPal[7][1]&0xFF; break;
								case 0x3B: return cBgPal[7][1]>>8; break;
								case 0x3C: return cBgPal[7][2]&0xFF; break;
								case 0x3D: return cBgPal[7][2]>>8; break;
								case 0x3E: return cBgPal[7][3]&0xFF; break;
								case 0x3F: return cBgPal[7][3]>>8; break;
							}
						}
						break;

						case OBPI:
						{
							return (autoIncObjPal<<7) + cObjpIndex + 0x40;
						}
						break;

						case OBPD:
						{
							switch(cObjpIndex)
							{
								case 0: return cObjPal[0][0]&0xFF; break;
								case 1: return cObjPal[0][0]>>8; break;
								case 2: return cObjPal[0][1]&0xFF; break;
								case 3: return cObjPal[0][1]>>8; break;
								case 4: return cObjPal[0][2]&0xFF; break;
								case 5: return cObjPal[0][2]>>8; break;
								case 6: return cObjPal[0][3]&0xFF; break;
								case 7: return cObjPal[0][3]>>8; break;
								case 8: return cObjPal[1][0]&0xFF; break;
								case 9: return cObjPal[1][0]>>8; break;
								case 0xA: return cObjPal[1][1]&0xFF; break;
								case 0xB: return cObjPal[1][1]>>8; break;
								case 0xC: return cObjPal[1][2]&0xFF; break;
								case 0xD: return cObjPal[1][2]>>8; break;
								case 0xE: return cObjPal[1][3]&0xFF; break;
								case 0xF: return cObjPal[1][3]>>8; break;
								case 0x10: return cObjPal[2][0]&0xFF; break;
								case 0x11: return cObjPal[2][0]>>8; break;
								case 0x12: return cObjPal[2][1]&0xFF; break;
								case 0x13: return cObjPal[2][1]>>8; break;
								case 0x14: return cObjPal[2][2]&0xFF; break;
								case 0x15: return cObjPal[2][2]>>8; break;
								case 0x16: return cObjPal[2][3]&0xFF; break;
								case 0x17: return cObjPal[2][3]>>8; break;
								case 0x18: return cObjPal[3][0]&0xFF; break;
								case 0x19: return cObjPal[3][0]>>8; break;
								case 0x1A: return cObjPal[3][1]&0xFF; break;
								case 0x1B: return cObjPal[3][1]>>8; break;
								case 0x1C: return cObjPal[3][2]&0xFF; break;
								case 0x1D: return cObjPal[3][2]>>8; break;
								case 0x1E: return cObjPal[3][3]&0xFF; break;
								case 0x1F: return cObjPal[3][3]>>8; break;
								case 0x20: return cObjPal[4][0]&0xFF; break;
								case 0x21: return cObjPal[4][0]>>8; break;
								case 0x22: return cObjPal[4][1]&0xFF; break;
								case 0x23: return cObjPal[4][1]>>8; break;
								case 0x24: return cObjPal[4][2]&0xFF; break;
								case 0x25: return cObjPal[4][2]>>8; break;
								case 0x26: return cObjPal[4][3]&0xFF; break;
								case 0x27: return cObjPal[4][3]>>8; break;
								case 0x28: return cObjPal[5][0]&0xFF; break;
								case 0x29: return cObjPal[5][0]>>8; break;
								case 0x2A: return cObjPal[5][1]&0xFF; break;
								case 0x2B: return cObjPal[5][1]>>8; break;
								case 0x2C: return cObjPal[5][2]&0xFF; break;
								case 0x2D: return cObjPal[5][2]>>8; break;
								case 0x2E: return cObjPal[5][3]&0xFF; break;
								case 0x2F: return cObjPal[5][3]>>8; break;
								case 0x30: return cObjPal[6][0]&0xFF; break;
								case 0x31: return cObjPal[6][0]>>8; break;
								case 0x32: return cObjPal[6][1]&0xFF; break;
								case 0x33: return cObjPal[6][1]>>8; break;
								case 0x34: return cObjPal[6][2]&0xFF; break;
								case 0x35: return cObjPal[6][2]>>8; break;
								case 0x36: return cObjPal[6][3]&0xFF; break;
								case 0x37: return cObjPal[6][3]>>8; break;
								case 0x38: return cObjPal[7][0]&0xFF; break;
								case 0x39: return cObjPal[7][0]>>8; break;
								case 0x3A: return cObjPal[7][1]&0xFF; break;
								case 0x3B: return cObjPal[7][1]>>8; break;
								case 0x3C: return cObjPal[7][2]&0xFF; break;
								case 0x3D: return cObjPal[7][2]>>8; break;
								case 0x3E: return cObjPal[7][3]&0xFF; break;
								case 0x3F: return cObjPal[7][3]>>8; break;
							}
						}
						break;

						default:
							printf("This address is not in gpu space!\n");
							return 0xFF;
						break;
					}
				}
				break;

				default:
					printf("This address is not in gpu space!\n");
					return 0xFF;
				break;
			}
		}
		break;

		default:
			printf("This address is not in gpu space!\n");
			return 0xFF;
		break;
	}
}

void GPU::write(u16 addr, u8 val)
{
	switch(addr>>12)
	{
		case 8: case 9:	
		{
			vram[vramSelect][addr & 0x1FFF] = val; 	
			if(addr <0x9800) 
			{
				updateTileData(addr&0x1FFF, val);
			}
			else
			{
				if(vramSelect == 1){ updateBgData(addr&0x1FFF, val); }
			}
		}
		break;

		case 15:
		{		
			switch((addr&0x0F00)>>8) 
			{				
				case 14:
				{
					oam[addr & 0xFF] = val;
					updateSpriteData(addr&0xFF, val);
				}
				break;
				
				case 15:
				{
					switch(addr&0xFF)
					{
						case LCDC: 
						{

							screenDisplay = (val&0x80)?1:0;
							windowTileMap = (val&0x40)?1:0;
							windowDisplay = (val&0x20)?1:0;
							tileDataLocation = (val&0x10)?1:0;
							bgTileMap = (val&0x08)?1:0;
							spriteSize16 = (val&0x04)?1:0;
							spriteDisplay = (val&0x02)?1:0;
							if((*gbMode) == NON_GBC_MODE){ bgwDisplay = (val&0x01)?1:0; }
							else{ bgwPrio = (val&0x01)?1:0; }
						} 
						break;

						case STAT: 
						{
							coincideIntr = (val&0x40)?1:0;
							mode2Intr = (val&0x20)?1:0;
							mode1Intr = (val&0x10)?1:0;
							mode0Intr = (val&0x08)?1:0;
						}
						break;

						case SCY: 
							bgy = val;
						break;

						case SCX: 
							bgx = val;
						break;

						case LY:
						{
							line = 0;
							mode = 0;
							lcd_cycle = 0;
						}
						break;
						
						case LYC:
							coincideLine = val;
						break;

						case BGP: 
							bgPal = val;
						break;
						
						case OBP0: 
							objPal0 = val;
						break;

						case OBP1: 
							objPal1 = val;
						break;

						case WY: 
							windowy = val;	
						break;

						case WX:
							windowx = val;
						break;

						case VBK:
							vramSelect = val&0x1;
						break;

						case BGPI:
						{
							cBgpIndex = val&0x3F;
							autoIncBgPal = (val&0x80)?1:0;
						}
						break;

						case BGPD:
						{
							switch(cBgpIndex)
							{
								case 0:    { cBgPal[0][0] = (cBgPal[0][0]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 1:    { cBgPal[0][0] = (cBgPal[0][0]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 2:    { cBgPal[0][1] = (cBgPal[0][1]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 3:    { cBgPal[0][1] = (cBgPal[0][1]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 4:    { cBgPal[0][2] = (cBgPal[0][2]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 5:    { cBgPal[0][2] = (cBgPal[0][2]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 6:    { cBgPal[0][3] = (cBgPal[0][3]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 7:    { cBgPal[0][3] = (cBgPal[0][3]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 8:    { cBgPal[1][0] = (cBgPal[1][0]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 9:    { cBgPal[1][0] = (cBgPal[1][0]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0xA:  { cBgPal[1][1] = (cBgPal[1][1]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0xB:  { cBgPal[1][1] = (cBgPal[1][1]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0xC:  { cBgPal[1][2] = (cBgPal[1][2]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0xD:  { cBgPal[1][2] = (cBgPal[1][2]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0xE:  { cBgPal[1][3] = (cBgPal[1][3]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0xF:  { cBgPal[1][3] = (cBgPal[1][3]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x10: { cBgPal[2][0] = (cBgPal[2][0]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x11: { cBgPal[2][0] = (cBgPal[2][0]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x12: { cBgPal[2][1] = (cBgPal[2][1]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x13: { cBgPal[2][1] = (cBgPal[2][1]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x14: { cBgPal[2][2] = (cBgPal[2][2]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x15: { cBgPal[2][2] = (cBgPal[2][2]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x16: { cBgPal[2][3] = (cBgPal[2][3]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x17: { cBgPal[2][3] = (cBgPal[2][3]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x18: { cBgPal[3][0] = (cBgPal[3][0]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x19: { cBgPal[3][0] = (cBgPal[3][0]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x1A: { cBgPal[3][1] = (cBgPal[3][1]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x1B: { cBgPal[3][1] = (cBgPal[3][1]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x1C: { cBgPal[3][2] = (cBgPal[3][2]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x1D: { cBgPal[3][2] = (cBgPal[3][2]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x1E: { cBgPal[3][3] = (cBgPal[3][3]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x1F: { cBgPal[3][3] = (cBgPal[3][3]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x20: { cBgPal[4][0] = (cBgPal[4][0]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x21: { cBgPal[4][0] = (cBgPal[4][0]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x22: { cBgPal[4][1] = (cBgPal[4][1]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x23: { cBgPal[4][1] = (cBgPal[4][1]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x24: { cBgPal[4][2] = (cBgPal[4][2]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x25: { cBgPal[4][2] = (cBgPal[4][2]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x26: { cBgPal[4][3] = (cBgPal[4][3]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x27: { cBgPal[4][3] = (cBgPal[4][3]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x28: { cBgPal[5][0] = (cBgPal[5][0]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x29: { cBgPal[5][0] = (cBgPal[5][0]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x2A: { cBgPal[5][1] = (cBgPal[5][1]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x2B: { cBgPal[5][1] = (cBgPal[5][1]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x2C: { cBgPal[5][2] = (cBgPal[5][2]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x2D: { cBgPal[5][2] = (cBgPal[5][2]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x2E: { cBgPal[5][3] = (cBgPal[5][3]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x2F: { cBgPal[5][3] = (cBgPal[5][3]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x30: { cBgPal[6][0] = (cBgPal[6][0]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x31: { cBgPal[6][0] = (cBgPal[6][0]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x32: { cBgPal[6][1] = (cBgPal[6][1]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x33: { cBgPal[6][1] = (cBgPal[6][1]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x34: { cBgPal[6][2] = (cBgPal[6][2]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x35: { cBgPal[6][2] = (cBgPal[6][2]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x36: { cBgPal[6][3] = (cBgPal[6][3]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x37: { cBgPal[6][3] = (cBgPal[6][3]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x38: { cBgPal[7][0] = (cBgPal[7][0]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x39: { cBgPal[7][0] = (cBgPal[7][0]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x3A: { cBgPal[7][1] = (cBgPal[7][1]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x3B: { cBgPal[7][1] = (cBgPal[7][1]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x3C: { cBgPal[7][2] = (cBgPal[7][2]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x3D: { cBgPal[7][2] = (cBgPal[7][2]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x3E: { cBgPal[7][3] = (cBgPal[7][3]&0xFF00) + val;		if(autoIncBgPal){cBgpIndex++;} } break;
								case 0x3F: { cBgPal[7][3] = (cBgPal[7][3]&0xFF) + (val<<8);		if(autoIncBgPal){cBgpIndex=0;} } break;
							}
						}
						break;

						case OBPI:
						{
							cObjpIndex = val&0x3F;
							autoIncObjPal = (val&0x80)?1:0;
						}
						break;

						case OBPD:
						{
							switch(cObjpIndex)
							{
								case 0:    { cObjPal[0][0] = (cObjPal[0][0]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 1:    { cObjPal[0][0] = (cObjPal[0][0]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 2:    { cObjPal[0][1] = (cObjPal[0][1]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 3:    { cObjPal[0][1] = (cObjPal[0][1]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 4:    { cObjPal[0][2] = (cObjPal[0][2]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 5:    { cObjPal[0][2] = (cObjPal[0][2]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 6:    { cObjPal[0][3] = (cObjPal[0][3]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 7:    { cObjPal[0][3] = (cObjPal[0][3]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 8:    { cObjPal[1][0] = (cObjPal[1][0]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 9:	   { cObjPal[1][0] = (cObjPal[1][0]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0xA:  { cObjPal[1][1] = (cObjPal[1][1]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0xB:  { cObjPal[1][1] = (cObjPal[1][1]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0xC:  { cObjPal[1][2] = (cObjPal[1][2]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0xD:  { cObjPal[1][2] = (cObjPal[1][2]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0xE:  { cObjPal[1][3] = (cObjPal[1][3]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0xF:  { cObjPal[1][3] = (cObjPal[1][3]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x10: { cObjPal[2][0] = (cObjPal[2][0]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x11: { cObjPal[2][0] = (cObjPal[2][0]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x12: { cObjPal[2][1] = (cObjPal[2][1]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x13: { cObjPal[2][1] = (cObjPal[2][1]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x14: { cObjPal[2][2] = (cObjPal[2][2]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x15: { cObjPal[2][2] = (cObjPal[2][2]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x16: { cObjPal[2][3] = (cObjPal[2][3]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x17: { cObjPal[2][3] = (cObjPal[2][3]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x18: { cObjPal[3][0] = (cObjPal[3][0]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x19: { cObjPal[3][0] = (cObjPal[3][0]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x1A: { cObjPal[3][1] = (cObjPal[3][1]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x1B: { cObjPal[3][1] = (cObjPal[3][1]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x1C: { cObjPal[3][2] = (cObjPal[3][2]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x1D: { cObjPal[3][2] = (cObjPal[3][2]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x1E: { cObjPal[3][3] = (cObjPal[3][3]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x1F: { cObjPal[3][3] = (cObjPal[3][3]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x20: { cObjPal[4][0] = (cObjPal[4][0]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x21: { cObjPal[4][0] = (cObjPal[4][0]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x22: { cObjPal[4][1] = (cObjPal[4][1]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x23: { cObjPal[4][1] = (cObjPal[4][1]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x24: { cObjPal[4][2] = (cObjPal[4][2]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x25: { cObjPal[4][2] = (cObjPal[4][2]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x26: { cObjPal[4][3] = (cObjPal[4][3]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x27: { cObjPal[4][3] = (cObjPal[4][3]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x28: { cObjPal[5][0] = (cObjPal[5][0]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x29: { cObjPal[5][0] = (cObjPal[5][0]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x2A: { cObjPal[5][1] = (cObjPal[5][1]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x2B: { cObjPal[5][1] = (cObjPal[5][1]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x2C: { cObjPal[5][2] = (cObjPal[5][2]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x2D: { cObjPal[5][2] = (cObjPal[5][2]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x2E: { cObjPal[5][3] = (cObjPal[5][3]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x2F: { cObjPal[5][3] = (cObjPal[5][3]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x30: { cObjPal[6][0] = (cObjPal[6][0]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x31: { cObjPal[6][0] = (cObjPal[6][0]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x32: { cObjPal[6][1] = (cObjPal[6][1]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x33: { cObjPal[6][1] = (cObjPal[6][1]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x34: { cObjPal[6][2] = (cObjPal[6][2]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x35: { cObjPal[6][2] = (cObjPal[6][2]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x36: { cObjPal[6][3] = (cObjPal[6][3]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x37: { cObjPal[6][3] = (cObjPal[6][3]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x38: { cObjPal[7][0] = (cObjPal[7][0]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x39: { cObjPal[7][0] = (cObjPal[7][0]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x3A: { cObjPal[7][1] = (cObjPal[7][1]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x3B: { cObjPal[7][1] = (cObjPal[7][1]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x3C: { cObjPal[7][2] = (cObjPal[7][2]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x3D: { cObjPal[7][2] = (cObjPal[7][2]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x3E: { cObjPal[7][3] = (cObjPal[7][3]&0xFF00) + val;		if(autoIncObjPal){cObjpIndex++;} } break;
								case 0x3F: { cObjPal[7][3] = (cObjPal[7][3]&0xFF) + (val<<8);	if(autoIncObjPal){cObjpIndex=0;} } break;
							}
						}
						break;

						default:
							printf("This address is not in gpu spacee!\n");
						break;
					}
				}
				break;

				default:
					printf("This address is not in gpu spacee!\n");
				break;
			}
		}
		break;

		default:
			printf("This address is not in gpu spacee!\n");
		break;
	}
}

Uint32 GPU::getRGB(u16 palData)
{
	int red = (palData&0x1F)*COLOR_RATIO;
	int green = ((palData>>5)&0x1F)*COLOR_RATIO;
	int blue = ((palData>>10)&0x1F)*COLOR_RATIO;
	return SDL_MapRGB(buffer->format, red, green, blue);
}

void GPU::close()
{
	SDL_FreeSurface( buffer );
    buffer = NULL;
	SDL_FreeSurface( big );
    big = NULL;

	SDL_FreeSurface( lcd );
	lcd = NULL;

    SDL_DestroyWindow( window );
    window = NULL;
	
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

	printf("SDL closed\n");
}
