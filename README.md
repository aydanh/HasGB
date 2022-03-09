# HasGB

A gameboy color emulator written in c++

Will run most gbc/gb games

I always wanted to write an emulator so I wrote this when I was at university. Don't plan on developing it further as there are already many great gbc emulators.

## **compiling:**

SDL2 is needed to compile the project:

    sudo apt-get install libsdl2-dev

to compile the project execute:

    make all

this will create an HasGB file

you can change the name in the Makefile

you may also need to update the include and library paths in the Makefile according to your SDL2 installation location

## **starting:**

to start the emulator execute

    /path/to/HasGB /path/to/gb-or-gbc-file

## **todo:**

- sound implementation
- add an application menu
- start games through application menu
- save and load states
- options for framerate etc
- implementing upscaling algorithms
- compatibility increase
