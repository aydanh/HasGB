#include "mmu.h"
#include <time.h>

MMU::MMU() {}
MMU::~MMU() {}

void MMU::init()
{	
	wramSelect = 1;
	int i;
	for(i=0; i<HRAM_SIZE; i++) 	{	hram[i] = 0;	for(int j=0; j<8; j++){ wram[j][i] = 0; } }
	for(i=HRAM_SIZE; i<RAM_SIZE; i++) {				for(int j=0; j<8; j++){ wram[j][i] = 0; } }

	intrFlag = &hram[IF];
	intrMask = &hram[IE];

	cartridge = 0;
	ramPresent = 0;
	battPresent = 0;;
	rumblePresent = 0;
	timePresent = 0;

	ramEnable = 0;
	ramMode = 0;
	selectl = 0;
	selecth = 0;
	selectr = 0;

	rom1 = NULL;
	eram = NULL;

	currentRomBank = 1;
	currentRamBank = 0;
	ramBankCount = 0;
	romBankCount = 0;

	speedMode = 0;
	speedSwitch = 0;

	hdmaSource = 0;
	hdmaDest = 0;
	hdmaLength = 0;
	hdmaMode = 0;
	hdmaActive = 0;

	unDoc6C = 0xFE;
	unDoc72 = 0;
	unDoc73 = 0;
	unDoc74 = 0;
	unDoc75 = 0x8F;
	unDoc76 = 0;
	unDoc77 = 0;

	rtc.seconds = 0;
	rtc.minutes = 0;
	rtc.hours = 0;
	rtc.lday = 0;
	rtc.hdayc = 0;

	dmaDone = 0;
}

void MMU::reset()
{
	wramSelect = 1;
	int i;
	for(i=0; i<HRAM_SIZE; i++) 	{	hram[i] = 0;	for(int j=0; j<8; j++){ wram[j][i] = 0; } }
	for(i=HRAM_SIZE; i<RAM_SIZE; i++) {				for(int j=0; j<8; j++){ wram[j][i] = 0; } }

	ramEnable = 0;
	ramMode = 0;
	selectl = 0;
	selecth = 0;
	selectr = 0;

	currentRomBank = 1;
	currentRamBank = 0;

	speedMode = 0;
	speedSwitch = 0;

	hdmaSource = 0;
	hdmaDest = 0;
	hdmaLength = 0;
	hdmaMode = 0;
	hdmaActive = 0;

	unDoc6C = 0xFE;
	unDoc72 = 0;
	unDoc73 = 0;
	unDoc74 = 0;
	unDoc75 = 0x8F;
	unDoc76 = 0;
	unDoc77 = 0;
}

void MMU::setGame(char* buffer, const char* gamepath)
{
	//printf("%#4x\n", buffer[0x143]);
	if(((buffer[0x143]&0xFF) == 0x80) || ((buffer[0x143]&0xFF) == 0xC0)){ gbMode = GBC_MODE; }
	else{ gbMode = NON_GBC_MODE; }

	if(gbMode == NON_GBC_MODE)
	{
		for(int i=0; i<16; i++) {
			game.title[i] = buffer[i+0x134];
		}
		unDoc74 = 0xFE;
	}
	else
	{
		for(int i=0; i<11; i++) {
			game.title[i] = buffer[i+0x134];
		}
		game.title[11] = '\0';
	}

	game.sgbFlag = buffer[0x146];
	game.cartType = buffer[0x147];
	game.romSize = buffer[0x148];
	game.ramSize = buffer[0x149];
	game.regionCode = buffer[0x14A];
	/*if(buffer[0x14B] == 0x33) { 
		game.licenceCode[0] = buffer[0x144];
		game.licenceCode[1] = buffer[0x145];
	}else{
		game.licenceCode[0] = buffer[0x14B];
		game.licenceCode[1] = 0x00;
	}*/
	//game.romVersion = buffer[0x14C];

	switch(game.cartType)
	{
		case 0:
			cartridge = 0;
		break;
		case 8:
		{
			cartridge = 0;
			ramPresent = 1;
			ramEnable = 1;
		}
		break;
		case 9:
		{
			cartridge = 0;
			ramPresent = 1;
			ramEnable = 1;
			battPresent = 1;
		}
		break;

		
		case 1:
			cartridge = 1;
		break;
		case 2: 
		{
			cartridge = 1;
			ramPresent = 1;
		}
		break;
		case 3:
		{
			cartridge = 1;
			ramPresent = 1;
			battPresent = 1;
		}
		break;

		
		case 5:
		{
			cartridge = 2;
		}
		break;
		case 6:
		{
			cartridge = 2;
			battPresent = 1;
		}
		break;


		case 0xF:
		{
			cartridge = 3;
			timePresent = 1;

		}
		break;
		case 0x10: 
		{
			cartridge = 3;
			timePresent = 1;
			ramPresent = 1;
			battPresent = 1;
		}
		break;
		case 0x11:
		{
			cartridge = 3;
		}
		break;
		case 0x12:
		{
			cartridge = 3;
			ramPresent = 1;
		}
		break;
		case 0x13:
		{
			cartridge = 3;
			ramPresent = 1;
			battPresent = 1;
		}
		break;
		
				
		case 0x19:
			cartridge = 4;
		break;
		case 0x1A:
		{
			cartridge = 4;
			ramPresent = 1;
		}
		break;
		case 0x1B:
		{
			cartridge = 4;
			ramPresent = 1;
			battPresent = 1;
		}
		break;
		case 0x1C:
		{
			cartridge = 4;
			rumblePresent = 1;
		}
		break;
		case 0x1D:
		{
			cartridge = 4;
			rumblePresent = 1;
			ramPresent = 1;
		}
		break;
		case 0x1E:
		{
			cartridge = 4;
			rumblePresent = 1;
			ramPresent = 1;
			battPresent  = 1;
		}
		break;

		
		case 0xFF:
			cartridge = 5;
			ramPresent = 1;
			battPresent = 1;
		break;
	}
	
	switch(game.romSize)
	{
		case 0:
		{
			rom1 = new u8 *[2];
			rom1[0] = rom0;
			rom1[1] = new u8[0x4000];
			romBankCount = 2;
		}
		break;
		
		case 1:
		{
			rom1 = new u8 *[4];
			rom1[0] = rom0;
			for(int i=1; i<4; i++){	rom1[i] = new u8[0x4000]; }
			romBankCount = 4;
		}
		break;
		
		case 2:
		{
			rom1 = new u8 *[8];
			rom1[0] = rom0;
			for(int i=1; i<8; i++){	rom1[i] = new u8[0x4000]; }
			romBankCount = 8;
		}
		break;
		
		case 3:
		{
			rom1 = new u8 *[16];
			rom1[0] = rom0;
			for(int i=1; i<16; i++){	rom1[i] = new u8[0x4000]; }
			romBankCount = 16;
		}
		break;
		
		case 4:
		{
			rom1 = new u8 *[32];
			rom1[0] = rom0;
			for(int i=1; i<32; i++){	rom1[i] = new u8[0x4000]; }
			romBankCount = 32;
		}
		break;
		
		case 5:
		{
			rom1 = new u8 *[64];
			rom1[0] = rom0;
			for(int i=1; i<64; i++){	rom1[i] = new u8[0x4000]; }
			romBankCount = 64;
		}
		break;
		
		case 6:
		{
			rom1 = new u8 *[128];
			rom1[0] = rom0;
			for(int i=1; i<128; i++){	rom1[i] = new u8[0x4000]; }
			romBankCount = 128;
		}
		break;

		case 7:
		{
			rom1 = new u8 *[256];
			rom1[0] = rom0;
			for(int i=1; i<256; i++){	rom1[i] = new u8[0x4000]; }
			romBankCount = 256;
		}
		break;
		
		case 0x52:
		{
			rom1 = new u8 *[72];
			rom1[0] = rom0;
			for(int i=1; i<72; i++){	rom1[i] = new u8[0x4000]; }
			romBankCount = 72;
		}
		break;
		
		case 0x53:
		{
			rom1 = new u8 *[80];
			rom1[0] = rom0;
			for(int i=1; i<80; i++){	rom1[i] = new u8[0x4000]; }
			romBankCount = 80;
		}
		break;
		
		case 0x54:
		{
			rom1 = new u8 *[96];
			rom1[0] = rom0;
			for(int i=1; i<96; i++){	rom1[i] = new u8[0x4000]; }
			romBankCount = 96;
		}
		break;
	}
	
	for(int i=0; i<0x4000; i++) {
		rom0[i] = buffer[i];
		rom1[1][i] = buffer[0x4000 + i];
	}
	for(int i=2; i<romBankCount; i++) 
	{
		for(int j=0; j<0x4000; j++)
		{
			rom1[i][j] = buffer[(i*0x4000) + j];
		}
	}
	//if(cartridge == 4){ currentRomBank = 0; }
	
	
	switch(game.ramSize)
	{
		case 0:
			ramBankCount = 0;
		break;
		
		case 1:
			ramBankCount = 1;
		break;
		
		case 2:
			ramBankCount = 1;
		break;
		
		case 3:
			ramBankCount = 4;
		break;
		
		case 4:
			ramBankCount = 8;
		break;
	}
	if(ramPresent && ramBankCount==0){ ramBankCount = 1; }
	if(ramBankCount > 0)
	{
		eram = new u8 *[ramBankCount];
		for(int i=0; i<ramBankCount; i++) 
		{
				eram[i] = new u8[0x2000];
				for(int j=0; j<0x2000; j++)
				{
					eram[i][j] = 0;
				}
		}

		if(battPresent)
		{
			const int len = strlen(gamepath) + 5;
			savepath = (char*)malloc(sizeof(char)*len);
			for(int i=0; i<strlen(gamepath); i++)
			{
				if(gamepath[i] != '.' && gamepath[i] != '\0')
				{
					savepath[i] = gamepath[i];
				}
				else
				{
					savepath[i]   = '.';
					savepath[i+1] = 's';
					savepath[i+2] = 'a';
					savepath[i+3] = 'v';
					savepath[i+4] = '\0';
					break;
				}


			}


			FILE* saveFile = fopen(savepath, "rb");
			if(!saveFile) { printf("No save found!\n"); }
			else
			{	
				fseek(saveFile, 0, SEEK_END);
				long saveSize = ftell(saveFile);
				rewind(saveFile);
				//printf("%d\n", saveSize);

				if(timePresent){ if(saveSize != (ramBankCount*0x2000 + 5)) { fclose(saveFile); printf("Invalid save file!\n"); return; } }
				else{  if(saveSize != (ramBankCount*0x2000)) { fclose(saveFile); printf("Invalid save file!\n"); return; } }
	
				char* sbuffer = (char*)malloc(sizeof(char)*saveSize);
				if(!sbuffer) { printf("Could not allocate memory for reading save file!\n"); }
	
				size_t result = fread(sbuffer,1,saveSize, saveFile);
				if(result != saveSize) { printf("Could not read save file!\n"); }

				for(int i=0; i<ramBankCount; i++)
				{
					for(int j=0; j<0x2000; j++)
					{
						eram[i][j] = sbuffer[i*0x2000 + j];
					}
				}
				if(timePresent)
				{
					rtc.seconds = sbuffer[(ramBankCount)*0x2000];
					rtc.minutes = sbuffer[(ramBankCount)*0x2000 + 1];
					rtc.hours = sbuffer[(ramBankCount)*0x2000 + 2];
					rtc.lday = sbuffer[(ramBankCount)*0x2000 + 3];
					rtc.hdayc = sbuffer[(ramBankCount)*0x2000 + 4];
					printf("%d %d %d %d %d\n", rtc.seconds, rtc.minutes, rtc.hours, rtc.lday, rtc.hdayc);
				}

				fclose(saveFile);
				free(sbuffer);
			}
		}

	}
}

u8 MMU::read(u16 addr)
{
	switch(addr>>12) 
	{
		case 0: case 1: case 2: case 3: 			//rom bank 0
			return rom0[addr];
		break;
		case 4: case 5: case 6: case 7: 	//rom bank 1
		{
			return rom1[currentRomBank][addr&0x3FFF];
		}
		break;
		case 8: case 9:						//video ram
			return gpu->read(addr); 	
		break;
		
		case 10: case 11:					//external ram
		{
			if(ramPresent) { return readExRam(addr&0x1FFF); }
			else{ printf("No exRam present for reading!\n"); return 0xFF; }
		}
		break;
		
		case 12: 					//working ram
			return wram[0][addr & 0x0FFF];
		break;

		case 13:					//working ram
			return wram[wramSelect][addr & 0x0FFF];
		break;
		
		case 14:							//echo of working ram
			return wram[0][addr & 0x0FFF];
		break;
		
		case 15:							//echo, oam, high ram
		{		
			switch((addr&0x0F00)>>8) 
			{
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: 	//echo ram
				case 8: case 9: case 10: case 11: case 12: case 13:
					return wram[wramSelect][addr & 0x0FFF];
				break;
				
				case 14:					//oam
				{
					if(addr<0xFEA0){ return gpu->read(addr); }
					else{ return 0xFF; }
				}
				break;
				
				case 15:	//high ram
				{
					switch(addr&0xFF)
					{
						case  P1:						//key
							return key->getKey();
						break;

						case SC:
							return hram[SC] | 0x7E; 
						break;

						case DIV: case TIMA: case TMA: case TAC:
							return timer->read(addr&0xFF);
						break;

						case IF:
							return (hram[IF]&0x1F) | 0xE0;
						break;

						case NR11: case NR12: case NR13: case NR14: case NR21: case NR22: case NR23:
						case NR24: case NR30: case NR31: case NR32: case NR33: case NR34: case NR41:
						case NR42: case NR43: case NR44: case NR50: case NR51: case NR52: case NR10:
							return audio->read(addr&0xFF);
						break;

						case LCDC: case STAT: case SCY: case SCX: case LY: case LYC:
						case BGP: case OBP0: case OBP1: case WY: case WX:
						case VBK: case BGPI: case BGPD: case OBPI: case OBPD:
							return gpu->read(addr);
						break;

						case WBK:
							return wramSelect;
						break;

						case SPD:
							return (((speedMode)?1:0)<<7) + speedSwitch + 0x7E;
						break;

						case DMASH:
							return (hdmaSource>>8);
						break;

						case DMASL:
							return (hdmaSource&0xFF);
						break;

						case DMADH:
							return (hdmaDest>>8);
						break;

						case DMADL:
							return (hdmaDest&0xFF);
						break;

						case DMAC:
							return ((hdmaActive?0:1)<<7)+((hdmaLength)&0x7F);
						break;

						case UD6C:
							return unDoc6C;
						break;

						case UD72:
							return unDoc72;
						break;

						case UD73:
							return unDoc73;
						break;

						case UD74:
							return unDoc74;
						break;

						case UD75:
							return unDoc75;
						break;

						case UD76:
							return unDoc76;
						break;

						case UD77:
							return unDoc77;
						break;

						case IE:
							return (hram[IE]&0x1F) | 0xE0;
						break;

						default:
							return hram[addr & 0xFF];
						break;
					}
				}
				break;
			}
		}
		break;

		default:
		{
			printf("No such address exist for reading!\n");
			return 0xFF;
		}
	}	
	return 0xFF;
}
	
u16 MMU::readB(u16 addr) 
{
	u16 n = read(addr);
	n += (read(addr+1)<<8);
	return n; 
}

void MMU::write(u16 addr, u8 val)
{
	switch(addr>>12) {
		case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
			chipControl(addr,val);
		break;
		
		case 8: case 9:			//video ram
			gpu->write(addr, val);
		break;
		
		case 10: case 11:					//external ram
		{
			if(ramPresent) { writeExRam(addr&0x1FFF, val); }
			else{ printf("No exRam present to write!\n"); }
		}
		break;
		
		case 12:					//working ram
			wram[0][addr & 0x0FFF] = val;
		break;

		case 13:					//working ram
			wram[wramSelect][addr & 0x0FFF] = val;
		break;
		
		case 14:							//echo of working ram
			printf("Can not write to echo ram!\n");
		break;
		
		case 15:	//echo, oam, high ram
		{		
			switch((addr&0x0F00)>>8) {
				case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: 	//echo ram
				case 8: case 9: case 10: case 11: case 12: case 13:
					printf("Can not write to echo ram!\n");
				break;
				
				case 14:						//oam				
					if(addr < 0xFEA0){ gpu->write(addr, val); }
				break;
				
				case 15:						//high ram
				{
					switch(addr&0xFF)
					{
						case P1:				//key
							key->setKeyState(val);
						break;

						case DIV: case TIMA: case TMA: case TAC:
							timer->write(addr&0xFF, val);
						break;
						/*
						case  SC:			//serial transfer
						{
							if(val&0x80){ printf("%c", (char)hram[SB]); }
							hram[SC] = val&0x7F;
							hram[SB] = 0x00;
							hram[IF] |= 0x80;
						}
						break;
						*/

						case NR11: case NR12: case NR13: case NR14: case NR21: case NR22: case NR23:
						case NR24: case NR30: case NR31: case NR32: case NR33: case NR34: case NR41:
						case NR42: case NR43: case NR44: case NR50: case NR51: case NR52: case NR10:
							audio->write(addr&0xFF,val);
						break;

						case LCDC: case STAT: case SCY: case SCX: case LY: case LYC:
						case BGP: case OBP0: case OBP1: case WY: case WX: 
						case BGPI: case BGPD: case OBPI: case OBPD:
							gpu->write(addr, val);
						break;

						case VBK:
						{
							if(gbMode == GBC_MODE)
							{
								//if(!hdmaActive)
								//{
									gpu->write(addr, val);
								//}
							}
						}
						break;

						case DMA:
							dma(val);
						break;

						case WBK:
						{
							if(gbMode == GBC_MODE)
							{
								//if(!hdmaActive || (hdmaSource<0xC000))
								//{
									wramSelect = val&0x7;
									if(wramSelect == 0){ wramSelect = 1; }
								//}
							}
						}
						break;

						case SPD:
						{
							if(gbMode == GBC_MODE)
							{
								speedSwitch = (val&0x1);
							}
						}
						break;

						case DMASH:
							if(!hdmaActive){ hdmaSource = (hdmaSource&0xF0) + ((val<<8)&0xFF00); }
						break;

						case DMASL:
							if(!hdmaActive){ hdmaSource = (hdmaSource&0xFF00) + (val&0xF0); }
						break;

						case DMADH:
							if(!hdmaActive){ hdmaDest = 0x8000 + (hdmaDest&0xF0) + ((val<<8)&0x1F00); }
						break;

						case DMADL:
							if(!hdmaActive){ hdmaDest = 0x8000 + (hdmaDest&0x1F00) + (val&0xF0); }
						break;

						case DMAC:
						{
							if(gbMode == GBC_MODE) 
							{ 
								if(!hdmaActive)
								{
									hdmaLength = val&0x7F;
									hdmaMode = (val>>7);
									if(hdmaMode == 1){ hdmaActive = 1; }
									else{ hdma(); }
								}
								else
								{
									if((val&0x80) == 0){ hdmaActive = 0; }
									hdmaLength = val&0x7F;
								}
							}
						}
						break;

						case UD6C:
							if(gbMode == GBC_MODE){	unDoc6C = 0xFE + val&0x1; }
						break;

						case UD72:
							unDoc72 = val;
						break;

						case UD73:
							unDoc73 = val;
						break;

						case UD74:
							if(gbMode == GBC_MODE){	unDoc74 = val; }
						break;

						case UD75:
							unDoc75 = 0x88 + ((val>>4)&0x7);
						break;

						case UD76:
						break;

						case UD77:
						break;

						default:
							hram[addr & 0xFF] = val;
						break;
					}
				}
				break;
			}
		}
		break;
		
		default:
			printf("No such address exist for writing!\n");
	}	
}

void MMU::writeB(u16 addr, u16 val)
{
	write(addr, (val&255));
	write(addr+1, ((val>>8)&255));
}

void MMU::dma(u16 addr)
{
	int i;
	u16 address = addr*0x100;
	if(address > 0xF100){ address = 0xF100; }
	for(i=0; i<OAM_SIZE; i++) {
		write((0xFE00 + i), read(address + i));
	}
	dmaDone=1;
}

void MMU::hdma()
{
	if(hdmaMode == 0)
	{
		//int t = 0;
		for(int i=0; (i<((hdmaLength+1)*0x10)); i++)
		{
			if(hdmaDest >= 0xA000){ hdmaDest = 0x8000; }
			if(hdmaSource >= 0x8000 && hdmaSource < 0xA000) { hdmaSource = 0xA000; }

			write((hdmaDest), read(hdmaSource));

			hdmaDest++;
			hdmaSource++;

			/*t++;
			if(t==10)
			{
				int cycle = 64/(speedMode+1);
				timer->tick(cycle, speedMode+1);
				gpu->tick(cycle);
				t = 0;
			}*/
			
		}

		hdmaLength = -1;
	}
	else
	{		
		for(int i=0; (i<0x10); i++)
		{
			write((hdmaDest+i), read(hdmaSource+i));
		}

		hdmaDest += 0x10;
		if(hdmaDest >= 0xA000){ hdmaDest = 0x8000; }

		hdmaSource += 0x10;
		if(hdmaSource >= 0x8000 && hdmaSource < 0xA000) { hdmaSource = 0xA000; }

		/*int cycle = (speedMode)?64:32;
		timer->tick(cycle, speedMode+1);
		gpu->tick(cycle);*/

		hdmaLength--;		
		if(hdmaLength<0)
		{
			hdmaActive = 0;
		}
	}
}

int MMU::getHDMAstate()
{
	return hdmaActive;
}

void MMU::bindPeripheral(GPU* n_gpu, TIMER* n_time, KEY* n_key, AUDIO* n_audio)
{
	gpu = n_gpu;
	timer = n_time;
	key = n_key;
	audio = n_audio;
}

void MMU::chipControl(u16 addr, u8 val)
{
	switch(cartridge)
	{
		case NONE:
			printf("Game tried to write to permanent rom!\n");
		break;
		
		case MBC1: case HuC1:
		{
			switch(addr>>12)
			{
				case 0: case 1: 
				{
					if((val&0xF) == 0xA) { ramEnable = 1; }
					else { ramEnable = 0; }
				}		
				break;
				
				case 2: case 3: 	
				{
					//if(!hdmaActive || (hdmaSource<0x4000 || hdmaSource>0x8000))
					//{
						selectl = val&0x1F;
						bankSelect();
					//}
				}
				break;
				
				case 4: case 5: 
				{
					if(ramMode == 0)
					{
						//if(!hdmaActive || (hdmaSource<0x4000 || hdmaSource>0x8000))
						//{
							selecth = val&0x3;
							bankSelect();
						//}
					}
					else
					{
						//if(!hdmaActive || (hdmaSource<0xA000 || hdmaSource>=0xC000))
						//{
							selecth = val&0x3;
							bankSelect();
						//}
					}
				}
				break;		
				
				case 6: case 7:
				{
					//if(!hdmaActive || selecth == 0 || (hdmaSource >=0xC000))
					//{
						ramMode = val&0x1;
						bankSelect();
					//}
				}
				break;
			}
		}
		break;
		
		case MBC2:
		{
			switch(addr>>12)
			{
				case 0: case 1:
				{
					if(!(addr&0x0100)) {
						if((val&0xF) == 0xA) { ramEnable = 1; }
						else { ramEnable = 0; }
					}
				}
				break;
				
				case 2: case 3:
				{
					//if(!hdmaActive || (hdmaSource<0x4000 || hdmaSource>0x8000))
					//{
						if(addr&0x0100) {
							selectl = (val&0xF);
							bankSelect();
						}
					//}
				}
				break;
				
				default:
					printf("This area for MBC2 is not writable!\n");
				break;
			}
		}
		break;
	
		case MBC3:
		{
			switch(addr>>12)
				{
					case 0: case 1: 
					{
						//if(!hdmaActive || (hdmaSource<0xA000 || hdmaSource>=0xC000))
						//{
							if((val&0xF) == 0xA) { ramEnable = 1; }
							else { ramEnable = 0; }
						//}
					}		
					break;
					
					case 2: case 3: 	
					{
						//if(!hdmaActive || (hdmaSource<0x4000 || hdmaSource>=0x8000))
						//{
							selectl = val&0x7F;
							bankSelect();
						//}
					}
					break;
					
					case 4: case 5: 
					{
						//if(!hdmaActive || (hdmaSource<0xA000 || hdmaSource>=0xC000))
						//{
							selecth = val&0xF;
							bankSelect();
						//}
					}
					break;		
					
					case 6: case 7:
					{
						if(timePresent)
						{
							if(ramMode == 0 && (val&0x1) == 1){
								ramMode = val&0x1;
								time_t rawTime;
								struct tm * timeInfo;
								time(&rawTime);
								timeInfo = localtime(&rawTime);
								rtc.seconds = (u8)timeInfo->tm_sec;
								rtc.minutes = (u8)timeInfo->tm_min;
								rtc.hours = (u8)timeInfo->tm_hour;
								rtc.lday = (u8)(timeInfo->tm_yday&255);
								rtc.hdayc = (u8)((timeInfo->tm_yday>>8)&0x1);
							}else{
								ramMode = val&0x1;
							}
						}
					}
					break;
				}
		}
		break;

		case MBC5:
		{
			switch(addr>>12)
			{
				case 0: case 1: 
				{
					if((val&0xF) == 0xA) { ramEnable = 1; }
					else 
					{ 
						//if(!hdmaActive || (hdmaSource<0xA000 || hdmaSource>=0xC000))
						//{
							ramEnable = 0;
						//}
					}
				}		
				break;
				
				case 2: 	
				{
					//if(!hdmaActive || (hdmaSource<0x4000 || hdmaSource>0x8000))
					//{
						selectl = val;
						bankSelect();
					//}
				}
				break;

				case 3: 	
				{
					//if(!hdmaActive || (hdmaSource<0x4000 || hdmaSource>0x8000))
					//{
						selecth = (val&0x1);
						bankSelect();
					//}
				}
				break;
				
				case 4: case 5: 
				{
					//if(!hdmaActive || (hdmaSource<0xA000 || hdmaSource>=0xC000))
					//{
						selectr = (val&0xF);
						bankSelect();
					//}
				}
				break;		
				
				case 6: case 7:
				{
					printf("This address is not writable on MBC5!\n");
				}
				break;
			}
		}
		break;
	}
}

void MMU::bankSelect()
{
	switch(cartridge)
	{
		case MBC1:
		{
			if(ramMode == 1) { 
				currentRamBank = (selecth&0x3);
				currentRomBank = (selectl&0x1F);
				if((selectl&0x1F) == 0) {currentRomBank = 1;}
			}else{
				currentRamBank = 0;
				currentRomBank = (selectl&0x1F) + ((selecth&0x3)<<5);
				if((selectl&0x1F) == 0) {currentRomBank++;}
			}
		}
		break;
		
		case MBC2:
		{
			currentRomBank = (selectl&0xF);
			if((selectl&0xF) == 0) {currentRomBank = 1;}
		}
		break;
		
		case MBC3:
		{
			if(timePresent)	{ if(selecth < 0x8) { currentRamBank =  (selecth&0x3); } }
			else{ currentRamBank =  (selecth&0x3); }
			currentRomBank = (selectl);
			if((selectl) == 0) { currentRomBank = 1;}
		}
		break;

		case MBC5:
		{
			currentRomBank = (selecth<<8) + selectl;
			//if(currentRomBank == 0){currentRomBank = 1;}
			if(rumblePresent)
			{
				currentRamBank =  (selectr&0x7);
			}
			else
			{
				currentRamBank =  (selectr&0xF);
			}
		}
		break;
	}
	currentRomBank %= (romBankCount);
	if(ramPresent){ currentRamBank %= (ramBankCount); }
}

u8 MMU::readExRam(u16 addr)
{
	if(timePresent && selecth >= 0x8)
	{
			switch(selecth&0xF)
			{
				case 0x8:
					return rtc.seconds;
				break;
					
				case 0x9:
					return rtc.minutes;
				break;
					
				case 0xA:
					return rtc.hours;
				break;
					
				case 0xB:
					return rtc.lday;
				break;
					
				case 0xC:
					return rtc.hdayc;
				break;
		}
	}else{
		return eram[currentRamBank][addr];
	}
}

void MMU::writeExRam(u16 addr, u8 val)
{
	if(ramEnable) 
	{
		if(timePresent && selecth >= 0x8)
		{
			if(rtc.hdayc&0x40 || ((selecth&0xF)==0xC)) 
			{
				switch(selecth&0xF)
				{
					case 0x8:
						if(rtc.hdayc&0x40){	rtc.seconds = val; }
					break;
						
					case 0x9:
						if(rtc.hdayc&0x40){	rtc.minutes = val; }
					break;
						
					case 0xA:
						if(rtc.hdayc&0x40){	rtc.hours = val; }
					break;
						
					case 0xB:
						if(rtc.hdayc&0x40){	rtc.lday = val; }
					break;
						
					case 0xC:
					{
						if(rtc.hdayc&0x40){	rtc.hdayc = val&0xC1; }
						else{ rtc.hdayc = rtc.hdayc&0x81 + val&0x40; }
					}
					break;
				}
			}
		}else{
			eram[currentRamBank][addr] = val;
		}
	}
}

void MMU::close()
{
	if(battPresent)
	{
		FILE* saveFile = fopen(savepath, "wb");
		if(!saveFile) { printf("Could not create save!\n"); }
		else
		{	
			rewind(saveFile);
			for(int i=0; i<ramBankCount; i++)
			{
				fwrite(eram[i],1,0x2000,saveFile);
			}
			if(timePresent)
			{
				fwrite(&rtc.seconds,1,1,saveFile);
				fwrite(&rtc.minutes,1,1,saveFile);
				fwrite(&rtc.hours,1,1,saveFile);
				fwrite(&rtc.lday,1,1,saveFile);
				fwrite(&rtc.hdayc,1,1,saveFile);
			}
			printf("save created\n");

		}
		free(savepath);
	}

	for(int i=0; i<ramBankCount; i++)
	{
		delete [] eram[i];
	}
	delete [] eram;

	for(int i=1; i<romBankCount; i++)
	{
		delete [] rom1[i];
	}
	delete [] rom1;
}
