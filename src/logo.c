#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "define.h"
#include "function.h"
#include "util_snd.h"
#include "extern.h"
#include "logo.h" 

// 関数プロトタイプ
void logo_init();
void logo_release();
void logo_keys();
void logo_draw();
void logo_init_save_data();

// 変数宣言
static bool scene_exit;
static int demo;

// メイン関数
void logo_main()
{
	logo_init( );	// 初期化
	
	while( !scene_exit )
	{
		logo_keys( );	// キー処理
		logo_draw( );	// 描画
		
		RefreshScreen();	// 描画
		FPSWait( );	// 待ち

		if ( system_keys( ) == 0 )
		{
			scene_exit = true;
		}
	}
	
	logo_release( );	// 終了
}

void logo_init( )
{
	scene_exit = false;
	demo = 0;
	
	LoadBitmap(IMAGE_PATH "a_logo.bmp",2,true);	//プレーンナンバー１にタイトル画面背景を読み込む

	soundStopBgm(EN_BGM_GAME01);
}

void logo_release()
{
	for (int i = 0; i < BMPBUFF_MAX; i++ )
		ReleaseBitmap( i );

	soundStopBgm(EN_BGM_GAME01);
}

void logo_keys( void )
{
	//	スペースキーが押される
	if ( ( IsPushKey(gameflag[4]) ) || ( IsPushKey(gameflag[5]) ) || ( demo > ( 60 * 3 ) ) )
	{
		gameflag[40] = 0;	/* タイトル */
		g_scene = EN_SN_TITLE;
		scene_exit = true;
		return;
	}
}

void logo_draw( void )
{
	//背景クリア
	ClearScreen();
	demo++;

	Blt( 2, 0, 0 );		/* 320 * 240 */

	//キー入力検査
	KeyInput();				
}
