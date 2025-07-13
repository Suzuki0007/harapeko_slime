#include "DxLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "scenegametitle.h"
#include "scenegamescenario.h"

SceneGameTitle::SceneGameTitle()
{
		// コンストラクタの初期化処理
	_cgBg = LoadGraph("res/Title.png"); // 背景画像の読み込み
	_next_image = LoadGraph("res/space_next.png"); // 次に進む画像の読み込み
}

SceneGameTitle::~SceneGameTitle()
{
	// デストラクタの後処理
	DeleteGraph(_cgBg); // 背景画像の解放
	DeleteGraph(_next_image); // 次に進む画像の解放
}

void SceneGameTitle::Input()
{
	// 入力処理
}

void SceneGameTitle::Process()
{
	// 処理
	if (gPad._trg& PAD_INPUT_10) // Aボタンが押されたら
	{
		SceneBase* scene = new SceneGameScenario(); // ゲームメインシーンを生成
		ChangeScene(scene); // ゲームメインシーンに変更
	}
}

void SceneGameTitle::Draw()
{
	// 描画処理
	DrawGraph(0, 0, _cgBg, FALSE); // 背景画像を描画
	DrawGraph(1600, 800, _next_image, TRUE); // 次に進む画像を描画

	//DrawFormatString(10, 10, GetColor(255, 255, 255),"SceneGameTitle!!"); // スタートメッセージを表示
}