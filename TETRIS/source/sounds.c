#include "sounds.h"

// Init the sdl mixer library
void initSounds()
{   
    pspAudioInit();

    sceUtilityLoadAvModule(PSP_AV_MODULE_MP3);
    sceMp3InitResource();
    SceMp3InitArg mp3Init;
    int handle = sceMp3ReserveMp3Handle( &mp3Init );
    sceMp3Init(handle);
    oslInit(0);
    oslInitAudio();
    oslInitAudioME(PSP_CODEC_MP3);
    oslInitAudioME(OSL_FMT_MP3);
}

// Load music from a file and return a pointer to it

OSL_SOUND *loadMusic(char *file)
{
    OSL_SOUND *music = oslLoadSoundFileMP3(file, OSL_FMT_STREAM);
    if(music == NULL)
    {
        printf("Unable to load music %s\n", file);
        exit(1);
    }
    return music;
}

OSL_SOUND *loadSFX(char *file)
{
    OSL_SOUND *sound = oslLoadSoundFileWAV(file, OSL_FMT_NONE);
    if(sound == NULL)
    {
        fprintf(stderr, "Unable to load WAV file: %s\n", file);
        exit(-1);
    }
    return sound;
}
