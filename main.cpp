#include <SDL2/SDL.h>
#include "hasgb.h"

int main(int argc, char* argv[])
{
	HASGB gb;
	if(!gb.init()) { gb.close(); exit(0); }

	if(!gb.loadGame(argv[1]))
	{
		gb.close();
		exit(0);
	}

	gb.printGameSpecs();
	
	SDL_Event e;
	SDL_PollEvent(&e);

	/*while(gb.getPC() != 0xb38)
	{
		gb.run();
	}*/

	do
	{
		gb.debugRun();
		//gb.run();
	}
	while(gb.handleEvent(e));
	
	gb.close();
	
	exit(0);
}