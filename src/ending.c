#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "define.h"
#include "function.h"
#include "util_snd.h"
#include "extern.h"
#include "ending.h" 

// 関数プロトタイプ
void ending_init();
void ending_release();
void ending_keys();
void ending_draw();
void ending_init_save_data();

// 変数宣言
static bool scene_exit;

// メイン関数
void ending_main()
{
	ending_init( );	// 初期化
	
	while( !scene_exit )
	{
		ending_keys( );	// キー処理
		ending_draw( );	// 描画
		
		RefreshScreen( );	// 描画
		FPSWait( );	// 待ち

		if ( system_keys( ) == 0 )
		{
			scene_exit = true;
		}
	}
	
	ending_release( );		// 終了
}

void ending_init()
{
	scene_exit = false;

	LoadBitmap(BAK_IMAGE_PATH "1.bmp",1,false);	//プレーンナンバー１にタイトル画面背景を読み込む
	LoadBitmap(END_IMAGE_PATH "1.bmp",2,true);	//プレーンナンバー１にタイトル画面背景を読み込む
	
	if ( gameflag[126] == 0 )
	{ 
		LoadBitmap(END_IMAGE_PATH "2.bmp",3,true);	//プレーンナンバー１にタイトル画面背景を読み込む
	}
	else 
	{
		LoadBitmap(END_IMAGE_PATH "3.bmp",3,true);	//プレーンナンバー１にタイトル画面背景を読み込む
	}

	/*LoadBitmap(SYS_IMAGE_PATH "waku.bmp",109,true);*/	//プレーンナンバー２にシステム用ＢＭＰを読み込む
	soundPlayBgm( EN_BGM_GAME02 );

}

void ending_release()
{
	for (int i = 0; i < BMPBUFF_MAX; i++ )
		ReleaseBitmap( i );

	soundStopBgm(EN_BGM_GAME01);
}


void ending_keys()
{
	//	スペースキーが押される
	if ( ( IsPushKey(gameflag[4]) ) || ( IsPushKey(gameflag[5]) ) )
	{
		gameflag[40] = 0;	/* タイトル */
		g_scene = EN_SN_TITLE;
		scene_exit = true;
		return;
	}

}

void ending_draw()
{
	ClearScreen();

	Blt( 1 , 0, 0 );
	Blt( 3 , 0, 0 );
	Blt( 2 , 0, 0 );
	
	//キー入力検査
	KeyInput();				
}
