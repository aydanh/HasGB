#include "key.h"

KEY::KEY() {}
KEY::~KEY() {}

void KEY::reset()
{
	keyState = 0xF0;
	key[0] = 0x0F;
	key[1] = 0x0F;
}

void KEY::setKeyState(u8 val)
{
	keyState = 0xC0 + val&0x30;
}

void KEY::setKey(int keyset, u8 val)
{
	key[keyset] &= val;
	switch(keyset)
	{
		case 0:
		{
			if(keyState == 2 || keyState == 0) 
			{ 
				(*intrFlag) |= 0x10; 
			}
		}
		break;

		case 1:
		{
			if(keyState == 1 || keyState == 0) 
			{ 
				(*intrFlag) |= 0x10; 
			}
		}
		break;
	}
}

void KEY::resetKey(int keyset, u8 val)
{
	key[keyset] |= val;
}

u8 KEY::getKey()
{
	switch(keyState)
	{
		case 0x20:
			return keyState + key[0];
		break;
		
		case 0x10:
			return keyState + key[1];
		break;
		
		case 0x00:
			return keyState + (key[0])&(key[1]);
		break;
		
		case 0x30:
			return 0x3F;
		break;
	}	
}