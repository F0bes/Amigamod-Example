#include "amigamod.h"
#include <stdio.h>
#include <debug.h> // Only used for basic text printing 
#include <kernel.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <string.h>
#include <unistd.h>
#include <libkbd.h>

// Holds the file buffer in EE memory, it'll get copied to IOP memory by amigamod
void* g_filebuffer;
int g_filebuffer_size = 0;

// Add whatever files you want to play here
const char* g_files[8] =
{
	"host:tunes/bubba3.mod",
	"host:tunes/bubba4.mod",
	"host:tunes/bubba5.mod",
	"host:tunes/bubba6.mod",
	"host:tunes/bubba7.mod",
	"host:tunes/bubba8.mod",
	"host:tunes/bubba9.mod",
	"host:tunes/cfodder.mod",
};
int g_files_size = 8;

void LoadFile(const char* file)
{
	FILE* fp;
	fp = fopen(file,"rb");
	
	fseek(fp,0,SEEK_END); // Seek to end and get the size
	int size = ftell(fp);
	
	fseek(fp,0,SEEK_SET); // Seek back to the start

	// If our existing memory buffer is big enough, just set it to zero and re-use it
	if(g_filebuffer_size < size) 
	{
		if(size) // If the buffer has been alloc'd before, free it
			free(g_filebuffer);
		g_filebuffer = malloc(size);
	}
	else
	{
		memset(g_filebuffer,0,size);
	}

	g_filebuffer_size = size;
	fread(g_filebuffer,8,size,fp); // Read file into the global buffer
	fclose(fp);
	return;
}

int main(void)
{
	// Required for ps2kbd
	SifLoadModule("host:usbd.irx",0,NULL);
	SifLoadModule("host:ps2kbd.irx",0,NULL);
	
	// Required for amigamod
	SifLoadModule("host:libsd.irx",0,NULL);
	SifLoadModule("host:amigamod.irx",0,NULL);

	// Init kbd 
	PS2KbdInit();
	PS2KbdSetReadmode(PS2KBD_READMODE_NORMAL);
	PS2KbdSetBlockingMode(PS2KBD_BLOCKING);

	// Init amigamod
	amigaModInit(0);

	// Init scr
	init_scr();
	
	// show some info
	scr_printf("		Amigamod test\n		Arrow up -> Volume up\n		Arrow down -> Volume down\n		Arrow right -> Next song\n		Arrow left -> Prev song\n");
		printf("		Amigamod test\n		Arrow up -> Volume up\n		Arrow down -> Volume down\n		Arrow right -> Next song\n		Arrow left -> Prev song\n");

	// Start the first file before going into our main loop
	LoadFile(g_files[0]);
	amigaModLoad(g_filebuffer,g_filebuffer_size);
	amigaModPlay(1);

	int i = 0;
	char key;
	int musicChanged = 0;
	unsigned short volume = 0x2FFF;
	while(1)
	{
		if(musicChanged){
			LoadFile(g_files[i]);
			amigaModLoad(g_filebuffer,g_filebuffer_size);
			amigaModPlay(1);
			musicChanged = 0;
		}

		PS2KbdRead(&key);
		if(key == 0x2a) // ARROW_LEFT
		{
			if(i == 0)
				i = g_files_size - 1;
			else
				i--;

			amigaModPause();
			musicChanged = 1;
		}
		else if(key == 0x29) // ARROW_RIGHT
		{
			if(i == g_files_size - 1)
				i = 0;
			else
				i++;

			amigaModPause();
			musicChanged = 1;
		}
		else if (key == 0x2c) // ARROW_UP
		{
			amigaModSetVolume(volume += 0x200);
		}
		else if (key == 0x2b) // ARROW_DOWN
		{
			amigaModSetVolume(volume -= 0x200);
		}
	}
	SleepThread();
}