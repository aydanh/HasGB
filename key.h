#ifndef _JOYPAD_H
#define _JOYPAD_H

#include <stdio.h>
#include <stdlib.h>

#ifndef BYTE_TYPE_DEFS
#define BYTE_TYPE_DEFS
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
#endif

class KEY
{
private:
	u8 keyState;
	u8 key[2];

public:
	u8* intrFlag;

public:
	KEY();
	~KEY();

	void reset();

	void setKeyState(u8 val);
	void setKey(int keyset, u8 val);
	void resetKey(int keyset, u8 val);
	u8	 getKey();
};

#endif