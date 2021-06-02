#include <debug.h>
#include <kernel.h>
#include <sbv_patches.h>
#include <loadfile.h>

#include "amigamod.h"

#include "libsd_irx.h"
#include "amigamod_irx.h"
#include "violent_death_mod.h"


int main(void)
{
	init_scr();
	sbv_patch_enable_lmb(); // Required for LoadModuleBuffer

	// The loads are not error checked
	SifExecModuleBuffer(libsd_irx,size_libsd_irx,0,NULL,NULL); // Load libsd
	SifExecModuleBuffer(amigamod_irx,size_amigamod_irx,0,NULL,NULL); // Load Amigamod
	
	amigaModInit(0); // Initialize amigamod

	amigaModLoad(violent_death_mod,size_violent_death_mod); // Load our song

	amigaModPlay(1); // Play the song
	// The music should be playing at this point, the below is just optional stuff
	// What's great about this library is that the EE can do whatever, and the IOP will
	// play the music all by itself !

	ModInfoStruct mis;
	amigaModGetInfo(&mis);
	scr_printf("	Song bpm: %d\n	Number of channels: %d\n	Current Row:     \n	Current Order:     ",mis.bpm,mis.numchannels);

	while(1)
	{
		amigaModGetInfo(&mis);
		scr_setXY(18,2);
		scr_printf("%d     ",mis.currow);
		scr_setXY(19,3);
		scr_printf("%d     ",mis.curorder);
	}

	SleepThread();
}