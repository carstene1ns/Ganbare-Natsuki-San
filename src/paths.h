#ifndef PATHS_H
#define PATHS_H

#include "define.h"

#ifndef DATA_PREFIX
#define DATA_PREFIX "data/"
#endif

#define SYS_IMAGE_PATH DATA_PREFIX "image/sys/"
#define IMAGE_PATH DATA_PREFIX "image/"
#define TITLE_IMAGE_PATH DATA_PREFIX "image/title/"
#define BAK_IMAGE_PATH DATA_PREFIX "image/bak/"
#define END_IMAGE_PATH DATA_PREFIX "image/end/"
#define ENM_IMAGE_PATH DATA_PREFIX "image/enm/"
#define PLAYER_IMAGE_PATH DATA_PREFIX "image/player/"
#define MAP_IMAGE_PATH DATA_PREFIX "image/map/"
#define MAP_DATA_PATH DATA_PREFIX "data/%d/"
#define OPTION_IMAGE_PATH DATA_PREFIX "image/option/"
#define BGM_SOUND_PATH DATA_PREFIX "sound/bgm/"
#define SE_SOUND_PATH DATA_PREFIX "sound/se/"

#define CONFIG_FIG_PATH "config/config.fig"

#endif // PATHS_H
