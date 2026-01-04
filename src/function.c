#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <SDL.h>
#include "define.h"
#include "function.h"
#include "extern.h"
#include "util_snd.h"
#include "general.h"
#include "dconv.h"

void KeyInit();

// キー取得用
static int key_eventPress[GP2X_BUTTON_MAX];
static int key_eventPress_old[GP2X_BUTTON_MAX];
static int key_eventPush[GP2X_BUTTON_MAX];
static int	pad_type;
#ifdef SDL_JOYSTICK
static int	pads;
static int	trgs;
static int	reps;
static SDL_Joystick *joys;
#endif
static const Uint8 *keys;

static SDL_Color palette_colors[256];

//static int pads_old;
// 画像表示用
static SDL_Texture *bitmap[BMPBUFF_MAX];
// 定時処理用
static Uint32 prvTickCount;
static Uint32 nowTick;
//static int frame;

#define INTERVAL_BASE 16

static SDL_Event event;

// 音楽再生
static Sint32 sound_vol;

void FunctionInit( void )
{
	int i;
	
	prvTickCount = 0;
	
	for ( i = 0; i < BMPBUFF_MAX; i++ )
	{
		bitmap[i] = NULL;
	}
	KeyInit( );
	soundInitBuffer();

}

void ResetGameFlag( void )
{
	memset( &gameflag[0], 0, sizeof( gameflag ) );
}
int LoadGameFlag( char *fn )
{
	int rc = 0;
#ifdef SDLFILE
	SDL_RWops *rw = SDL_RWFromFile(fn, "rb");
	if (!rw)
	{
		rc = -1;
	}
	else 
	{
		SDL_RWread(rw, &gameflag[0], sizeof( gameflag ), 1);
		SDL_RWclose(rw);
	}
#else
	FILE *fp;
	if ( ( fp = fopen( fn, "rb" ) ) == NULL )
	{
		printf("file open error!! %s\n", fn);
		rc = -1;	
	}
	else 
	{
		fread( &gameflag[0], 1, sizeof( gameflag ), fp );
		fclose( fp );
	}
#endif
	
	return ( rc );
}
int SaveGameFlag( char *fn )
{
	int rc = 0;
#ifdef SDLFILE
	SDL_RWops *rw = SDL_RWFromFile(fn, "wb");
	if (!rw)
	{
		rc = -1;
	}
	else 
	{
		SDL_RWwrite(rw, &gameflag[0], 1, sizeof( gameflag ));
		SDL_RWclose(rw);
	}
#else
	FILE *fp;
	if ( ( fp = fopen( fn, "wb" ) ) == NULL )
	{
		printf("file open error!! %s\n", fn);
		rc = -1;
	}
	else 
	{
		fwrite( &gameflag[0], 1, sizeof( gameflag ), fp );
		fclose( fp );
	}
#endif
	
	return ( rc );
}

void ResetGameFlag2( void )
{
	memset( &gameflag2[0], 0, sizeof( gameflag ) );
}

int LoadGameFlag2( char *fn )
{
	int rc = 0;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	int32_t tmp, i;
#endif

#ifdef SDLFILE
	SDL_RWops *rw = SDL_RWFromFile(fn, "rb");
	if (!rw)
	{
		rc = -1;
	}
	else 
	{
		SDL_RWread(rw, &gameflag2[0], sizeof( gameflag ), 1);
		SDL_RWclose(rw);
	}
#else
	FILE *fp;
	if ( ( fp = fopen( fn, "rb" ) ) == NULL )
	{
		printf("file open error!! %s\n", fn);
		rc = -1;	
	}
	else 
	{
		printf("Fopen %s was a sucess\n", fn);
		fread( &gameflag2[0], 1, sizeof( gameflag ), fp ); 
		fclose( fp );	
	}
#endif
	
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	for(i=0;i<GAMEFLAG_SIZE;i++)
	{
		tmp = gameflag2[i];
		gameflag2[i] = __builtin_bswap32(tmp);
	}
	#endif
	
	return ( rc );
}

int SaveGameFlag2( char *fn )
{
	int rc = 0;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Sint32 bgameflag2[GAMEFLAG_SIZE], i;
#endif

#ifdef SDLFILE
	SDL_RWops *rw = SDL_RWFromFile(fn, "wb");
	if (!rw)
	{
		printf("file open error!! %s\n", fn);
		rc = -1;
	}
	else 
	{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		memcpy(bgameflag2, gameflag2, sizeof(bgameflag2));
		for(i=0;i<GAMEFLAG_SIZE;i++)
		{
			bgameflag2[i] = __builtin_bswap32(gameflag2[i]);
		}
		SDL_RWwrite(rw, &bgameflag2[0], 1, sizeof(gameflag));
#else
		SDL_RWwrite(rw, &gameflag2[0], 1, sizeof(gameflag));
#endif
		SDL_RWclose(rw);
	}
#else
	FILE *fp;
	if ( ( fp = fopen( fn, "wb" ) ) == NULL )
	{
		printf("file open error!! %s\n", fn);
		rc = -1;
	}
	else 
	{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		memcpy(bgameflag2, gameflag2, sizeof(bgameflag2));
		for(i=0;i<GAMEFLAG_SIZE;i++)
		{
			bgameflag2[i] = __builtin_bswap32(gameflag2[i]);
		}
		fwrite( &bgameflag2[0], 1, sizeof( gameflag ), fp ); 
#else
		fwrite( &gameflag2[0], 1, sizeof( gameflag ), fp ); 
#endif
		fclose( fp );
	}
#endif
	
	return ( rc );
}
int SaveFile( char *fn, Sint32 *buff, Sint32 size )
{
	int rc = 0;
#ifdef SDLFILE
	SDL_RWops *rw = SDL_RWFromFile(fn, "wb");
	if (!rw)
	{
		printf("file open error!! %s\n", fn);
		rc = -1;
	}
	else 
	{
		SDL_RWwrite(rw, buff, 1,size);
		SDL_RWclose(rw);
	}
#else
	FILE *fp;	

	if ( ( fp = fopen( fn, "wb" ) ) == NULL )
	{
		printf("file open error!! %s\n", fn);
		rc = -1;
	}
	else 
	{
		fwrite( buff, 1, size, fp ); 
		fclose( fp );
	}
#endif

	return ( rc );
}
int LoadFile( char *fn, Sint32 *buff, Sint32 size )
{
	int rc = 0;
#ifdef SDLFILE
	SDL_RWops *rw = SDL_RWFromFile(fn, "rb");
	if (!rw)
	{
		printf("file open error!! %s\n", fn);
		rc = -1;
	}
	else 
	{
		SDL_RWread(rw, buff, size, 1);
		SDL_RWclose(rw);
	}
#else
	FILE *fp;	
	if ( ( fp = fopen( fn, "rb" ) ) == NULL )
	{
		printf("file open error!! %s\n", fn);
		rc = -1;
	}
	else 
	{
		fread( buff, 1, size, fp ); 
		fclose( fp );
	}
#endif
	
	return ( rc );
}

Sint32 LoadBitmap( char *fname , int bmpindex, bool transparent )
{
	ReleaseBitmap(bmpindex);

	SDL_Surface *tmp = SDL_LoadBMP(fname);
	if (!tmp)
	{
		printf("Could not find %s\n", fname);
		return -1;
	}

	if(SDL_ISPIXELFORMAT_INDEXED(tmp->format->format))
		SDL_SetPaletteColors(tmp->format->palette, palette_colors, 0, 256);

	if (transparent)
		SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 16, 99, 62));

	SDL_Texture *tex = SDL_CreateTextureFromSurface(g_renderer, tmp);
	SDL_FreeSurface(tmp);
	if(!tex)
	{
		printf("Could not create texture %s\n", fname);
		return -1;
	}

	bitmap[bmpindex] = tex;
	return 0;
}

// BMPの開放
void ReleaseBitmap(int bmpindex)
{
	if (bitmap[bmpindex] != NULL)
	{
		SDL_DestroyTexture(bitmap[bmpindex]);
		bitmap[bmpindex] = NULL;
	}
}

// ＢＭＰ範囲指定表示
void BltRect(int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height)
{
	SDL_Rect srcRect;
	SDL_Rect dstRect;
	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = width;
	srcRect.h = height;
	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = width;
	dstRect.h = height;

	if(bitmap[bmpindex])
		SDL_RenderCopy(g_renderer, bitmap[bmpindex], &srcRect, &dstRect);
}

// ＢＭＰ範囲指定表示
void Blt( int bmpindex, int dstX, int dstY )
{
	SDL_Rect dstRect;
	dstRect.x = dstX;
	dstRect.y = dstY;

	if(!bitmap[bmpindex])
		return;

	if(SDL_QueryTexture(bitmap[bmpindex], NULL, NULL, &dstRect.w, &dstRect.h) != 0)
		return;

	SDL_RenderCopy(g_renderer, bitmap[bmpindex], NULL, &dstRect);
}

void SetGscreenPalette(SDL_Surface *surface)
{
	if(!surface)
		return;

	if(surface->format->BitsPerPixel != 8)
		return;

	for(int i = 0; i < 256; i++)
		palette_colors[i] = surface->format->palette->colors[i];
}

void ClearScreen()
{
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 0xff);
	SDL_RenderClear(g_renderer);
}

// 背景のクリア
void RefreshScreen()
{
	SDL_RenderPresent(g_renderer);
}

void KeyInit()
{
	int i;
	
	sound_vol = 128;
	 
	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		key_eventPress[i] = 0;
		key_eventPress_old[i] = 0;
		key_eventPush[i] = 0;
	}

	initPAD( );
}

int initPAD(void)
{
#ifdef SDL_JOYSTICK
	if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0){
		return 0;
    }

	if(SDL_NumJoysticks() > 0)
	{
		joys = SDL_JoystickOpen(0);
		SDL_JoystickEventState(SDL_ENABLE);
	}
	else
	{
		joys = NULL;
	}

	pad_type = 0;
	trgs = 0;
	reps = 0;
#endif
	return 1;
}

void closePAD(void)
{
#ifdef SDL_JOYSTICK
	if(SDL_JoystickOpened(0))
		SDL_JoystickClose(joys);
#endif
}

void KeyInput()
{
	int i;
	int pad = 0;

	keys = SDL_GetKeyboardState(NULL);

#ifdef SDL_JOYSTICK
	int	btn1 = 0, btn2 = 0, btn3 = 0, btn4 = 0, btn5 = 0, btn6 = 0, btn7 = 0, btn8 = 0, btn9 = 0, btnA = 0;
	int x, y;
	if(joys){
		x = SDL_JoystickGetAxis(joys, 0);
		y = SDL_JoystickGetAxis(joys, 1);
	}
#endif

	if(pad_type == 0)
	{
		if(keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_KP_6]
		#ifdef SDL_JOYSTICK
		 || x > JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_RIGHT;
		}
		if(keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_KP_4]
		#ifdef SDL_JOYSTICK
		|| x < -JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_LEFT;
		}
		if(keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_KP_2]
		#ifdef SDL_JOYSTICK
		|| y > JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_DOWN;
		}
		if(keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_KP_8]
		#ifdef SDL_JOYSTICK
		|| y < -JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_UP;
		}
	}
	else if(pad_type == 1)
	{
		if(keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_KP_6]
		#ifdef SDL_JOYSTICK
		|| x > JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_RIGHT;
		}
		if(keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_KP_4]
		#ifdef SDL_JOYSTICK
		|| x < -JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_LEFT;
		}
		if(keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_KP_2]
		#ifdef SDL_JOYSTICK
		|| y > JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_DOWN;
		}
		if(keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_KP_8]
		#ifdef SDL_JOYSTICK
		|| y < -JOYSTICK_AXIS
		#endif
		){
			pad |= PAD_UP;
		}
	}

#ifdef SDL_JOYSTICK
	if(joys){
		btn1 = SDL_JoystickGetButton(joys, 0);
		btn2 = SDL_JoystickGetButton(joys, 1);
		btn3 = SDL_JoystickGetButton(joys, 2);
		btn4 = SDL_JoystickGetButton(joys, 3);
		btn5 = SDL_JoystickGetButton(joys, 4);
		btn6 = SDL_JoystickGetButton(joys, 5);
		btn7 = SDL_JoystickGetButton(joys, 6);
		btn8 = SDL_JoystickGetButton(joys, 7);
		btn9 = SDL_JoystickGetButton(joys, 8);
		btnA = SDL_JoystickGetButton(joys, 9);
	}
#endif	
	if(pad_type == 0)
	{
		if(keys[SDL_SCANCODE_LCTRL]
		|| keys[SDL_SCANCODE_X]
#ifdef SDL_JOYSTICK
		 || btn1
#endif
		 ){
			pad |= PAD_BUTTON1;
		}
		if(keys[SDL_SCANCODE_LALT]
		|| keys[SDL_SCANCODE_SPACE]
		|| keys[SDL_SCANCODE_C]
#ifdef SDL_JOYSTICK
		 || btn2
#endif
		){
			pad |= PAD_BUTTON2;
		}
		if(keys[SDL_SCANCODE_RETURN]
		#ifdef SDL_JOYSTICK
		|| btn3
		#endif
		){
			pad |= PAD_BUTTON3;
		}
	}
	else if(pad_type == 1)
	{
		if(keys[SDL_SCANCODE_BACKSLASH]
#ifdef SDL_JOYSTICK
		 || btn1
#endif
		){
			pad |= PAD_BUTTON1;
		}
		if(keys[SDL_SCANCODE_RSHIFT]
#ifdef SDL_JOYSTICK
		 || btn2
#endif
		){
			pad |= PAD_BUTTON2;
		}
		if(keys[SDL_SCANCODE_P]
		#ifdef SDL_JOYSTICK
		|| btn3
		#endif
		){
			pad |= PAD_BUTTON3;
		}
	}

	if(keys[SDL_SCANCODE_F1]
		#ifdef SDL_JOYSTICK
		|| btn7
		#endif
	){
		pad |= PAD_BUTTON7;
	}
	if(keys[SDL_SCANCODE_F2]
#ifdef SDL_JOYSTICK
		 || btn8
#endif
	){
		pad |= PAD_BUTTON8;
	}
	
#ifdef SDL_JOYSTICK
	if(btn4){
		pad |= PAD_BUTTON4;
	}
	if(btn5){
		pad |= PAD_BUTTON5;
	}
	if(btn6){
		pad |= PAD_BUTTON6;
	}
	if(btn9){
		pad |= PAD_BUTTON9;
	}
	if(btnA){
		pad |= PAD_BUTTONA;
	}
#endif
	
	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		key_eventPress[i] = 0;
	}
	
	if ( pad & PAD_UP )
	{
		key_eventPress[GP2X_BUTTON_UP] = 1;
	}
	if ( pad & PAD_DOWN )
	{
		key_eventPress[GP2X_BUTTON_DOWN] = 1;
	}
	if ( pad & PAD_LEFT )
	{
		key_eventPress[GP2X_BUTTON_LEFT] = 1;
	}
	if ( pad & PAD_RIGHT )
	{
		key_eventPress[GP2X_BUTTON_RIGHT] = 1;
	}
	if ( pad & PAD_BUTTON1 )
	{
		key_eventPress[GP2X_BUTTON_A] = 1;
	}
	if ( pad & PAD_BUTTON2 )
	{
		key_eventPress[GP2X_BUTTON_X] = 1;
	}
	if ( pad & PAD_BUTTON3 )
	{
		key_eventPress[GP2X_BUTTON_Y] = 1;
	}
	if ( pad & PAD_BUTTON4 )
	{
		key_eventPress[GP2X_BUTTON_B] = 1;
	}
	if ( pad & PAD_BUTTON5 )
	{
		key_eventPress[GP2X_BUTTON_R] = 1;
	}
	if ( pad & PAD_BUTTON6 )
	{
		key_eventPress[GP2X_BUTTON_L] = 1;
	}
	if ( pad & PAD_BUTTON7 )
	{
		key_eventPress[GP2X_BUTTON_VOLDOWN] = 1;
	}
	if ( pad & PAD_BUTTON8 )
	{
		key_eventPress[GP2X_BUTTON_VOLUP] = 1;
	}
	if ( pad & PAD_BUTTON9 )
	{
		key_eventPress[GP2X_BUTTON_SELECT] = 1;
	}
	if ( pad & PAD_BUTTONA )
	{
		key_eventPress[GP2X_BUTTON_START] = 1;
	}
	if ( pad & PAD_BUTTONB )
	{
		key_eventPress[GP2X_BUTTON_CLICK] = 1;
	}
	if ( keys[SDL_SCANCODE_ESCAPE] )	// 終了
	{
		key_eventPress[GP2X_BUTTON_EXIT] = 1;
	}
	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		if ( ( key_eventPress_old[i] == 0 ) && ( key_eventPress[i] != 0 ) )
		{
			key_eventPush[i] = 1;
		}
		else 
		{
			key_eventPush[i] = 0;
		}
		key_eventPress_old[i] = key_eventPress[i];
		
	}
}

int IsPushKey( int keycode )
{
	int rc = 0;
	
	if ( key_eventPush[keycode] == 1 )
	{
		rc = 1;
	}

	return( rc );
}

int IsPressKey( int keycode )
{
	int rc = 0;
	
	if ( key_eventPress[keycode] == 1 )
	{
		rc = 1;
	}

	return( rc );
}

int IsPushOKKey( void )
{
	int rc = 0;
	if ( key_eventPush[GP2X_BUTTON_A] == 1 )
	{
		rc = 1;
	}

	return( rc );
}
int IsPushCancelKey( void )
{
	int rc = 0;
	
	if ( key_eventPush[GP2X_BUTTON_X] == 1 )
	{
		rc = 1;
	}

	return( rc );
}

void FPSWait( void )
{
	Uint32 leftTick;

	//サウンドの制御
	soundPlayCtrl( );
	SDL_PollEvent(&event);

	if(prvTickCount == 0) prvTickCount = SDL_GetTicks();
	
	while (true)
	{
	 	nowTick = SDL_GetTicks();
 		leftTick = prvTickCount + INTERVAL_BASE - nowTick;
 		if(leftTick < 1 || leftTick > 9999)
 		{
			break;
		}

		SDL_Delay(1);
	}
	prvTickCount = nowTick;

	gameflag[107] = gameflag[107] + 1;
	if ( gameflag[107] >= 60 )
	{
		gameflag[107] = 0;
		gameflag[108] = gameflag[108] + 1;
		if ( gameflag[108] >= 60 )
		{
			gameflag[108] = 0;
			gameflag[109] = gameflag[109] + 1;
			if ( gameflag[109] >= 60 )
			{
				gameflag[109] = 0;
				gameflag[110] = gameflag[110] + 1;
				if ( gameflag[110] >= 999 )
				{
					gameflag[110] = 999;
				}
			}
		}
	}

}

int system_keys()
{
	int rc;
	
	rc = 1;
	if ( ( event.type == SDL_QUIT ) || ( IsPressKey( GP2X_BUTTON_EXIT ) ) )
	{
		rc = 0;
		g_scene = EN_SN_EXIT;
	}
	// 音量調整
	if ( IsPushKey( GP2X_BUTTON_VOLUP ) )
	{
		gameflag[60] = gameflag[60] + 10;
		if ( gameflag[60] > VOL_MAX )
		{
			gameflag[60] = VOL_MAX;
		}
		Set_Volume( gameflag[60] );
	}
	if ( IsPushKey( GP2X_BUTTON_VOLDOWN ) )
	{
		gameflag[60] = gameflag[60] - 10;
		if ( gameflag[60] < 0 )
		{
			gameflag[60] = 0;
		}
		Set_Volume( gameflag[60] );
	}
	
	return( rc );
}
int Set_Volume( int vol )
{
	int rc = 0;
	
	soundSetVolumeBgm( vol, 0 );
	soundSetVolumeAll( vol );
	
	return( rc );
}

/*[ BltNumericImage ]************************************************/
/*	数値画像表示関数												*/
/*-[引数]-----------------------------------------------------------*/
/*	value			(i )	画像表示する数値						*/
/*	length			(i )	表示する桁数（桁数以上は表示されない）	*/
/*	x				(i )	画像を表示する位置ｘ座標				*/
/*	y				(i )	画像を表示する位置ｙ座標				*/
/*	plane			(i )	数値画像が読み込まれているプレーン番号	*/
/*	num_stpos_x		(i )	プレーン内での数値画像の開始位置ｘ座標	*/
/*	num_stpos_y		(i )	プレーン内での数値画像の開始位置ｙ座標	*/
/*	num_width		(i )	数値画像１文字の幅（ドット数）			*/
/*	num_height		(i )	数値画像１文字の高さ（ドット数）		*/
/*-[戻り値]---------------------------------------------------------*/
/*	無し															*/

void BltNumericImage( Sint32 value, Sint32 length, Sint32 x, Sint32 y, Sint32 plane, Sint32 num_stpos_x, Sint32 num_stpos_y, Sint32 num_width, Sint32 num_height )
{
	Sint32 blt_num;	// １桁の数値を格納する
	Sint32 i;			// 桁数分のforループで使用
	Sint32 dv;		// 割り算で使用する値

	// value が負の値の場合、正の値に置き換える
	if ( value < 0 )
	{
		value = value * -1;
	}

	// 最初の割り算で使用する値を求める
	dv = 1;
	for( i = 1; i < length; i++ )
	{
		dv = dv * 10;
	}

	// 指定された桁数分の数字画像を転送する
	for( i = 0; i < length; i++ )
	{
		// 表示する数字を求める
		blt_num = value / dv;
		value = value - blt_num * dv;
		if ( blt_num > 9 )
		{	// 表示したい１桁の数値にならなければ、１桁にする。
			blt_num = blt_num % 10;
		}
		// 数字画像転送
		BltRect( plane, x + (num_width * i), y, num_stpos_x + (num_width * blt_num), num_stpos_y, num_width, num_height );
		// 割り算で使用する値を10で割る
		dv = dv / 10;
	}

	return;
}

/*[ BltNumericImage2 ]************************************************/
/*	数値画像表示関数（右詰め）										*/
/*-[引数]-----------------------------------------------------------*/
/*	value			(i )	画像表示する数値						*/
/*	length			(i )	表示する桁数（桁数以上は表示されない）	*/
/*	x				(i )	画像を表示する位置ｘ座標				*/
/*	y				(i )	画像を表示する位置ｙ座標				*/
/*	plane			(i )	数値画像が読み込まれているプレーン番号	*/
/*	num_stpos_x		(i )	プレーン内での数値画像の開始位置ｘ座標	*/
/*	num_stpos_y		(i )	プレーン内での数値画像の開始位置ｙ座標	*/
/*	num_width		(i )	数値画像１文字の幅（ドット数）			*/
/*	num_height		(i )	数値画像１文字の高さ（ドット数）		*/
/*-[戻り値]---------------------------------------------------------*/
/*	無し															*/
/********************************************************************/
void BltNumericImage2( Sint32 value, Sint32 length, Sint32 x, Sint32 y, Sint32 plane, Sint32 num_stpos_x, Sint32 num_stpos_y, Sint32 num_width, Sint32 num_height )
{
	Sint32 blt_num;	// １桁の数値を格納する
	Sint32 i;			// 桁数分のforループで使用
	Sint32 dv;		// 割り算で使用する値
	Sint32 x_hosei;	//右詰め補正値
	int buf;
	int t = 1;
	
	// value が負の値の場合、正の値に置き換える
	if ( value < 0 )
	{
		value = value * -1;
	}
	
	/* 2002.10.21 D.K start */
	buf = value;
	
	while(true)
	{
		t++;
		buf = buf / 10;
		if ( 0 <= buf )
		{
			break;
		}
	}
	x_hosei = t;
//	scanf(string[0],"%d",value);
//	x_hosei = StrLen( string[0] );
	if ( x_hosei == 0 )
	{
		x_hosei = 1;
	}
	x_hosei = length - x_hosei;
	x_hosei = x_hosei * num_width;
//	length = StrLen( string[0] );
	x = x + x_hosei;
	/* 2002.10.21 D.K end */
	
	// 最初の割り算で使用する値を求める
	dv = 1;
	for( i = 1; i < length; i++ )
	{
		dv = dv * 10;
	}

	// 指定された桁数分の数字画像を転送する
	for( i = 0; i < length; i++ )
	{
		// 表示する数字を求める
		blt_num = value / dv;
		value = value - blt_num * dv;
		if ( blt_num > 9 )
		{	// 表示したい１桁の数値にならなければ、１桁にする。
			blt_num = blt_num % 10;
		}
		// 数字画像転送
		BltRect( plane, x + (num_width * i), y, num_stpos_x + (num_width * blt_num), num_stpos_y, num_width, num_height );
		// 割り算で使用する値を10で割る
		dv = dv / 10;
	}

	return;
}

Sint32 funcSin( Sint32 rdo )
{
	Sint32 ang;
	Sint32 rc = 0;
	
	if ( ( rdo >= 0 ) && ( rdo < 180 ) )
	{
		ang = ( ( 65535 * rdo ) / 360 );
		rc = MOTsin( ang );
	}
	if ( ( rdo >= 180 ) && ( rdo < 360 ) )
	{
		rdo = rdo - 180;
		ang = ( ( 65535 * rdo ) / 360 );
		rc = MOTsin( ang );
		rc = rc * -1;
	}
	
	return( rc );
}
Sint32 funcCos( Sint32 rdo )
{
	Sint32 ang;
	Sint32 rc = 0;
	
	if ( ( rdo >= 0 ) && ( rdo < 180 ) )
	{
		ang = ( ( 65535 * rdo ) / 360 );
		rc = MOTcos( ang );
	}
	if ( ( rdo >= 180 ) && ( rdo < 360 ) )
	{
		rdo = rdo - 180;
		ang = ( ( 65535 * rdo ) / 360 );
		rc = MOTcos( ang );
		rc = rc * -1;
	}
	
	return( rc );
}


Sint32 funcTan2( Sint32 posX, Sint32 posY )
{
	Sint32 rc = 0;
	
	rc = MOTatan( ( posX ) * 256, ( posY * -1 ) * 256 );
	
	return( rc );
}

Sint32 get2keta( Sint32 val, Sint32 st )
{
	Sint32 rc = 0;
	
	val = val / st;
	rc = val % 100;
	
	return( rc );
}

