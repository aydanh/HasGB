#include "hasgb.h"

HASGB::HASGB() {}
HASGB::~HASGB() {}

int HASGB::init()
{
	if(SDL_Init(NULL) != 0)
	{
		printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	timer.reset();
	key.reset();
	if(!gpu.init()){ return 0; }
	gpu.reset();
	//if(!audio.init()){ return 0; }
	audio.reset();
	mmu.init();
	cpu.reset();

	mmu.bindPeripheral(&gpu, &timer, &key, &audio);
	cpu.setMem(&mmu);
	gpu.intrFlag = mmu.intrFlag;
	timer.intrFlag = mmu.intrFlag;
	key.intrFlag = mmu.intrFlag;
	gpu.gbMode = &mmu.gbMode;

	comm = 0;
	valid = 0;
	nextPC = 0x0;
	dbg = 0;

	return 1;
}


void HASGB::reset()
{
	gpu.reset();
	audio.reset();
	key.reset();
	timer.reset();
	mmu.reset();
	cpu.reset();
}

int HASGB::loadGame(const char* gamepath)
{
	FILE* gameFile = fopen(gamepath, "rb");
	if(!gameFile) { printf("File could not be opened!\n"); return 0; }
	
	fseek(gameFile, 0, SEEK_END);
	long fileSize = ftell(gameFile);
	rewind(gameFile);
	
	char* buffer = (char*)malloc(sizeof(char)*fileSize);
	if(!buffer) { printf("Could not allocate memory!\n"); return 0; }
	
	size_t result = fread(buffer,1,fileSize, gameFile);
	if(result != fileSize) { printf("Could not read file!\n"); return 0; }

	mmu.setGame(buffer, gamepath);
	
	fclose(gameFile);
	free(buffer);
	
	return 1;
}

int HASGB::handleEvent(SDL_Event e)
{
	while(SDL_PollEvent(&e) != 0){
		if(e.type == SDL_QUIT){
			return 0;
		}else if(e.type == SDL_KEYDOWN) {
			switch(e.key.keysym.sym) {
				case SDLK_ESCAPE: { return 0; } break;
				case SDLK_TAB: { reset(); } break;
				case SDLK_z: { key.setKey(1, 0x0E); } break;
				case SDLK_x: { key.setKey(1, 0x0D); } break;
				case SDLK_RETURN: { key.setKey(1, 0x07);  } break;
				case SDLK_RSHIFT: { key.setKey(1, 0x0B);  } break;
				case SDLK_UP: { key.setKey(0, 0x0B); } break;
				case SDLK_DOWN: { key.setKey(0, 0x07);  } break;
				case SDLK_LEFT: { key.setKey(0, 0x0D); } break;
				case SDLK_RIGHT: { key.setKey(0, 0x0E); } break;
				case SDLK_LCTRL: { dbg = 1; } break;
			}
		} else if(e.type == SDL_KEYUP) {
			switch(e.key.keysym.sym) {
				case SDLK_z: { key.resetKey(1, 0x01); } break;
				case SDLK_x: {  key.resetKey(1, 0x02); } break;
				case SDLK_RETURN: { key.resetKey(1, 0x08);  } break;
				case SDLK_RSHIFT: {  key.resetKey(1, 0x04);  } break;
				case SDLK_UP: {  key.resetKey(0, 0x04); } break;
				case SDLK_DOWN: {  key.resetKey(0, 0x08);  } break;
				case SDLK_LEFT: {  key.resetKey(0, 0x02); } break;
				case SDLK_RIGHT: {  key.resetKey(0, 0x01); } break;
			}
		}
	}
	return 1;
}

void HASGB::run()
{
	int cycle = cpu.step();

	/*if(mmu.dmaDone)
	{
		int t = 640;
		while(t>0){ 
			int a = cpu.step();
			timer.tick(a, cpu.getSpeedMode());
			gpu.tick(a);
			t -= a;
		}

		mmu.dmaDone = 0;
	}*/

	timer.tick(cycle, cpu.getSpeedMode());

	int gpuState = gpu.tick(cycle);
	if(!gpuState){ printf("Could not execute gpu operation!\n"); }
	else if((gpuState == 2) && mmu.getHDMAstate() /*&& cpu.getHalt()*/){ mmu.hdma(); }

	//audio.tick(cycle);
}

void HASGB::debugRun()
{
	if(dbg)
	{
	if(nextPC >0)
	{
		run();
		nextPC-=1;
	}
	else
	{
		cpu.printState();

		while(!valid)
		{
			scanf("%d", &comm);
			switch(comm)
			{
				case 1:
					nextPC = 1;
					valid = 1;
				break;
				case 2:
				{
					int z = cpu.getNextInstAddr();

					while(cpu.getPC() != z)
					{
						run();
					}
					cpu.printState();

				}
				break;	
				case 3:
				{
					run();
					while(getPC() != 0x3095)
					{
						run();
					}
					cpu.printState();
				}
				break;
				case 0:
					dbg = 0;
				break;
				default:
					while(getPC() != comm)
					{
						run();
					}
					cpu.printState();
				break;
			}
		}
		valid = 0;
	}
	}
	else
	{
		run();
	}
}


void HASGB::printGameSpecs()
{
	int i;
	printf("Title: ");
	if(mmu.gbMode != GBC_MODE)
	{
		for(i=0; i<16; i++) {
			if(mmu.game.title[i] != 0x00){
				printf("%c", mmu.game.title[i]);
			}else{ break; }
		}
	}
	else
	{
		for(i=0; i<11; i++) {
			if(mmu.game.title[i] != 0x00){
				printf("%c", mmu.game.title[i]);
			}else{ break; }
		}
	}
	printf("\n");

	printf("ColorGB: ");
	if(mmu.gbMode == GBC_MODE){ printf("Yes\n"); } else { printf("No\n"); }
	
	printf("SuperGB: ");
	if(mmu.game.sgbFlag){ printf("Yes\n"); } else { printf("No\n"); }
	
	printf("Cartridge Type: ");
	switch(mmu.game.cartType)
	{
		case 0:
			printf("Rom Only\n");
		break;
		
		case 1:
			printf("Rom + MBC1\n");
		break;
		
		case 2:
			printf("Rom + MBC1 + Ram\n");
		break;
		
		case 3:
			printf("Rom + MBC1 + Ram + Battery\n");
		break;
		
		case 5:
			printf("Rom + MBC2 + Battery\n");
		break;
		
		case 8:
			printf("Rom + Ram\n");
		break;
		
		case 9:
			printf("Rom + Ram + Battery\n");
		break;
		
		case 0xB:
			printf("Rom + MMMO1\n");
		break;
		
		case 0xC:
			printf("Rom + MMMO1 + Sram\n");
		break;
		
		case 0xD:
			printf("Rom + MMMO1 + Sram + Battery\n");
		break;
		
		case 0xF:
			printf("Rom + MBC3 + Timer + Battery\n");
		break;
		
		case 0x10:
			printf("Rom + MBC3 + Timer + Ram + Battery\n");
		break;
		
		case 0x11:
			printf("Rom + MBC3\n");
		break;
		
		case 0x12:
			printf("Rom + MBC3 + Ram\n");
		break;
		
		case 0x13:
			printf("Rom + MBC3 + Ram + Battery\n");
		break;
		
		case 0x19:
			printf("Rom + MBC5\n");
		break;
		
		case 0x1A:
			printf("Rom + MBC5 + Ram\n");
		break;
		
		case 0x1B:
			printf("Rom + MBC5 + Ram + Battery\n");
		break;
		
		case 0x1C:
			printf("Rom + MBC5 + Rumble\n");
		break;
		
		case 0x1D:
			printf("Rom + MBC5 + Rumble + Sram\n");
		break;
		
		case 0x1E:
			printf("Rom + MBC5 + Rumble + Sram + Battery\n");
		break;
		
		case 0x1F:
			printf("Pocket Camera\n");
		break;
		
		case 0xFD:
			printf("Bandai TAMA5\n");
		break;
		
		case 0xFE:
			printf("Hudson HuC-3\n");
		break;
		
		case 0xFF:
			printf("Hudson HuC-1\n");
		break;
	}
	
	printf("Rom Size: ");
	switch(mmu.game.romSize)
	{
		case 0:
			printf("32KByte");
		break;
		
		case 1:
			printf("64KByte");
		break;
		
		case 2:
			printf("128KByte");
		break;
		
		case 3:
			printf("256KByte");
		break;
		
		case 4:
			printf("512KByte");
		break;
		
		case 5:
			printf("1MByte");
		break;
		
		case 6:
			printf("2MByte");
		break;
		
		case 0x52:
			printf("1.1MByte");
		break;
		
		case 0x53:
			printf("1.2MBte");
		break;
		
		case 0x54:
			printf("1.5MByte");
		break;
	}
	printf("\n");
	
	printf("Ram Size: ");
	switch(mmu.game.ramSize)
	{
		case 0:
			printf("None");
		break;
		
		case 1:
			printf("2KByte");
		break;
		
		case 2:
			printf("8KByte");
		break;
		
		case 3:
			printf("32KByte");
		break;
		
		case 4:
			printf("128KByte");
		break;
	}
	printf("\n");
	
	printf("Region: ");
	if(mmu.game.regionCode) { printf("Non-Japanese\n"); }
	else { printf("Japanese\n"); }
	
	printf("\n");
}

u16 HASGB::getPC()
{
	return cpu.getPC();
}

void HASGB::close()
{
	mmu.close();
	gpu.close();
	//audio.close();
	SDL_Quit();
}