#include "DxLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "gamepad.h"
#include "scenegamescenario.h"
#include "scenegamemain.h"

extern GamePad gPad;								// ゲームパッドの情報

SceneGameScenario::SceneGameScenario()
{
	_pageNo = 0;									// 現在のシナリオ番号を初期化

	_cgMap = LoadGraph("res/Stage_01_01.png");		//背景
	_next_image = LoadGraph("res/space_next.png");	// 次に進む画像の読み込み

	// シナリオテキスト画像の読み込み
	for (int i = 0; i < SCENARIO_MAX; i++)
	{
		char filename[999];
		sprintf(filename, "res/textbox_%d.png", i + 1);
		_scenario_text_image[i] = LoadGraph(filename);
	}

	// 女の子立ち絵の読み込み
	for (int i = 0; i < SCENARIO_GIRL; i++)
	{
		char filename[999];
		sprintf(filename, "res/girl_%d.png", i + 1);
		_scenario_girl[i] = LoadGraph(filename);
	}

	// スライム立ち絵の読み込み
	for (int i =0; i< SCENARIO_SLIME; i++)
	{
		char filename[999];
		sprintf(filename, "res/slime_%d.png", i + 1);
		_scenario_slime[i] = LoadGraph(filename);
	}

	cgMap_X = 0;										// cgMapのX座標
	cgMap_Y = 0;										// cgMapのY座標

	scenario_text_image_X = 0;							// シナリオテキスト画像のX座標
	scenario_text_image_Y = 0;							// シナリオテキスト画像のY座標

	next_image_X = 1600;								// 次に進む画像のX座標
	next_image_X = 800;									// 次に進む画像のY座標

	scenario_girl_X = 1160;								// 女の子立ち絵のX座標
	scenario_girl_Y = 200;								// 女の子立ち絵のY座標

	scenario_slime_X = 120;								// スライム立ち絵のX座標
	scenario_slime_Y = 200;								// スライム立ち絵のY座標

}

SceneGameScenario::~SceneGameScenario()
{
	// 画像の解放
	DeleteGraph(_cgMap);
	DeleteGraph(_next_image);

	// シナリオテキスト画像の解放
	for (int i = 0; i < SCENARIO_MAX; i++)
	{
		DeleteGraph(_scenario_text_image[i]);
	}

	//少女の子立ち絵の解放
	for (int i = 0; i < SCENARIO_GIRL; i++)
	{
		DeleteGraph(_scenario_girl[i]);
	}

	// スライム立ち絵の解放
	for (int i = 0; i < SCENARIO_SLIME; i++)
	{
		DeleteGraph(_scenario_slime[i]);
	}


}

void SceneGameScenario::Input()
{
	// 何もしない
}

void SceneGameScenario::Process()
{
	int padNo = 0;
	if (gPad._m[padNo]._trg & PAD_INPUT_9)
	{
		if (_pageNo < SCENARIO_MAX - 1)
		{
			_pageNo++;								// ページを進める
		}
		else
		{
			SceneBase* scene = new SceneGameMain(); // シーンをゲームメインに切り替え
			ChangeScene(scene);						// シーンを変更
		}
	}
}

void SceneGameScenario::Draw()
{
	// 描画処理
	switch (_pageNo)
	{
		case 0:// シナリオ1ページ目
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[0], TRUE); // シナリオテキスト画像0を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE);								// 次に進む画像を描画
			break;
		}
		case 1://シナリオ2ページ目
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE);				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[1], TRUE);					// 女の子立ち絵1を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[1], TRUE); // シナリオテキスト画像1を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE);								// 次に進む画像を描画
			break;
		}
		case 2:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE);				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[1], TRUE);					// 女の子立ち絵1を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[2], TRUE); // シナリオテキスト画像2を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 3:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE);				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[4], TRUE);					// 女の子立ち絵4を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[3], TRUE); // シナリオテキスト画像3を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE);								// 次に進む画像を描画
			break;
		}
		case 4:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE);				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[4], TRUE);					// 女の子立ち絵4を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[4], TRUE);	// シナリオテキスト画像4を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE);								// 次に進む画像を描画
			break;
		}
		case 5:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE);				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[4], TRUE);					// 女の子立ち絵4を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[5], TRUE); // シナリオテキスト画像5を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE);								// 次に進む画像を描画
			break;
		}
		case 6:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE);				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[3], TRUE);					// 女の子立ち絵3を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[6], TRUE); // シナリオテキスト画像6を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE);								// 次に進む画像を描画
			break;
		}
		case 7:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE); 				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[4], TRUE); 					// 女の子立ち絵4を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[7], TRUE); // シナリオテキスト画像7を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 8:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE); 				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[1], TRUE); 					// 女の子立ち絵1を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[8], TRUE); // シナリオテキスト画像8を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 9:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE); 												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[1], TRUE); 				// スライム立ち絵1を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[1], TRUE); 					// 女の子立ち絵1を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[9], TRUE); // シナリオテキスト画像9を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 10:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE); 												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[1], TRUE); 				// スライム立ち絵1を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[4], TRUE); 					// 女の子立ち絵4を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[10], TRUE);// シナリオテキスト画像10を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 11:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[3], TRUE); 				// スライム立ち絵3を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[4], TRUE); 					// 女の子立ち絵4を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[11], TRUE);// シナリオテキスト画像11を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 12:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE); 												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[3], TRUE); 				// スライム立ち絵3を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[3], TRUE); 					// 女の子立ち絵3を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[12], TRUE);// シナリオテキスト画像12を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 13:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE); 												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[3], TRUE); 				// スライム立ち絵3を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[1], TRUE); 					// 女の子立ち絵1を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[13], TRUE);// シナリオテキスト画像13を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 14:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE); 												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[3], TRUE); 				// スライム立ち絵3を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[0], TRUE); 					// 女の子立ち絵0を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[14], TRUE);// シナリオテキスト画像14を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 15:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE); 				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[3], TRUE); 					// 女の子立ち絵3を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[15], TRUE);// シナリオテキスト画像15を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE);								// 次に進む画像を描画
			break;
		}
		case 16:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE); 												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE);				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[4], TRUE); 					// 女の子立ち絵4を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[16], TRUE);// シナリオテキスト画像16を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE);								// 次に進む画像を描画
			break;
		}
		case 17:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE); 												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE); 				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[4], TRUE); 					// 女の子立ち絵4を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[17], TRUE);// シナリオテキスト画像17を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 18:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE); 				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[3], TRUE); 					// 女の子立ち絵3を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[18], TRUE);// シナリオテキスト画像18を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 19:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE);												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE); 				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[1], TRUE); 					// 女の子立ち絵1を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[19], TRUE);// シナリオテキスト画像19を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		case 20:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE); 												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE); 				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[3], TRUE);					// 女の子立ち絵3を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[20], TRUE);// シナリオテキスト画像20を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE);								// 次に進む画像を描画
			break;
		}
		case 21:
		{
			DrawGraph(cgMap_X, cgMap_Y, _cgMap, TRUE); 												// 背景画像を描画
			DrawGraph(scenario_slime_X, scenario_slime_Y, _scenario_slime[0], TRUE); 				// スライム立ち絵0を描画
			DrawGraph(scenario_girl_X, scenario_girl_Y, _scenario_girl[4], TRUE); 					// 女の子立ち絵4を描画
			DrawGraph(scenario_text_image_X, scenario_text_image_Y, _scenario_text_image[21], TRUE);// シナリオテキスト画像21を描画
			DrawGraph(next_image_X, next_image_Y, _next_image, TRUE); 								// 次に進む画像を描画
			break;
		}
		break;
	}
}
