TARGET = Tutorial
OBJS   = main.o graphics.o sounds.o scenes.o

PSPDEV=$(shell psp-config -d)
PSPBIN=$(PSPDEV)/psp/bin
PSPSDK=$(shell psp-config --pspsdk-path)

CFLAGS = -O3 -Wall 
CFLAGS += $(shell sdl2-config --cflags)
CXXFLAGS = $(CFLAGS) -fno-rtti
ASFLAGS  = $(CFLAGS)

LIBS = $(shell $(PSPBIN)/sdl2-config --libs)
LIBS +=  $(shell sdl2-config --libs)
LIBS += -losl -lpspmp3 -lpspaudiocodec -lSDL2_image -lSDL2_gfx -lpng -ljpeg -lz -lvorbis -logg  -lvorbisfile -lsmpeg  -lmad -lpspaudiolib -lm
BUILD_PRX = 1
PSP_LARGE_MEMORY = 1

EXTRA_TARGETS   = EBOOT.PBP
PSP_EBOOT_TITLE = TETRIS
PSP_EBOOT_ICON= ICON0.png
PSP_EBOOT_PIC1= PIC1.png

include $(PSPSDK)/lib/build.mak
