# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

EE_BIN = amigamod_test.elf
EE_OBJS = main.o amigamod_rpc.o 
EE_LIBS = -lkernel -lpatches -ldebug
BIN2C = bin2c
BIN2C_TARGETS = libsd_irx.h amigamod_irx.h violent_death_mod.h

all: $(BIN2C_TARGETS) $(EE_BIN)

libsd_irx.h:
	$(BIN2C) irx/libsd.irx libsd_irx.h libsd_irx

amigamod_irx.h:
	$(BIN2C) irx/amigamod.irx amigamod_irx.h amigamod_irx

violent_death_mod.h:
	$(BIN2C) violent_death.mod violent_death_mod.h violent_death_mod

clean:
	rm -f $(EE_BIN) $(EE_OBJS) $(BIN2C_TARGETS)

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal