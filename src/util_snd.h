#ifndef UTIL_SND_H
#define UTIL_SND_H

#include <stdbool.h>

#define SOUND_MUSBANK 32
#define SOUND_CHUNKBANK 32
#define SOUND_MIXBANK 8

extern void soundInitBuffer();
extern void soundRelease();
extern void soundLoadBuffer(Sint32 num, Uint8 *fname, int loop);
extern void soundLoadBuffer2(Sint32 num, Uint8 *fname1, Uint8 *fname2);
extern void soundLoadBufferSE(Sint32 num, Uint8 *fname);
extern void soundStopBgm(Sint32 num);
extern void soundStopBgmPlaying();
extern bool soundIsPlayBgm();
extern void soundPlayBgm(Sint32 num);
extern void soundPlayFadeFlag(Sint32 flag, Sint32 time);
extern void soundPlayCtrl();
extern void soundSetVolumeMaster(Sint32 vol);
extern void soundSetVolumeBgm(Sint32 vol, Sint32 num);
extern void soundSetVolumeAll(Sint32 vol);
extern void soundStopSe(Sint32 num);
extern void soundPlaySe(Sint32 num);
extern bool soundIsPlaySe(Sint32 num);
extern void soundStopSeAll();

#endif // UTIL_SND_H
