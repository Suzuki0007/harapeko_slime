#include "DxLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "scenebase.h"
#include "scenegametitle.h"
#include "scenegamemain.h"

GamePad gPad; // ゲームパッドの情報

//シーンポインタ
SceneBase* gScene; //ポインタなのでnewするまで実体は無い

SceneBase* gNewScene; //新しいシーンのポインタ

void ChangeScene(SceneBase* scene)
{
	gNewScene = scene; // 新しいシーンを設定
}

void AppInit()
{
	gNewScene = NULL; // 新しいシーンのポインタを初期化
	gScene = new SceneGameTitle(); // 初期シーンをタイトルに設定
}

void AppRelease()
{
	delete gScene; // 現在のシーンを解放
}

void FrameInput()
{
	gPad.Input(); // ゲームパッドの入力を更新

	gScene->Input(); // 現在のシーンの入力処理を呼び出す
}

void FrameProcess()
{
	gScene->Process(); // 現在のシーンの処理を呼び出す
}

void FrameDraw()
{

	ClearDrawScreen(); // 描画画面をクリア

	gScene->Draw(); // 現在のシーンの描画処理を呼び出す

	ScreenFlip(); // 描画画面を裏画面に反映
}

void GameMain()
{
	AppInit(); // アプリの初期化

	// 1フレームループを組む ----------------------------------------------------------
	while (ProcessMessage() == 0)		// プログラムが終了するまでループ
	{
		// シーンを切り替えるか？
		if (gNewScene != NULL) {
			delete gScene;		// 今のシーンを削除する
			gScene = gNewScene;	// 受け取ったシーンをポインタセット
			gNewScene = NULL;	// 次のシーンを初期化する
		}

		FrameInput();	// 入力
		FrameProcess();	// 計算
		FrameDraw();	// 描画
	}

	AppRelease();	// 解放
}