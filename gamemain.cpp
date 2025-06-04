#define _CRT_SECURE_NO_WARNINGS
#include	"DxLib.h"


//
// 定数・構造体定義
//


struct PlayerStatus {
	int current_hp;	// 現在のHP
	int max_hp;		// 最大HP
	int hp_step;	// 画像を切り替えるHPの間隔
	int num_img;	// 画像の枚数
};

#define SCREEN_W	(1920)		// 画面の横解像度
#define SCREEN_H	(1080)		// 画面の縦解像度

// チップのサイズ
#define	CHIP_W		(64)
#define	CHIP_H		(64)

// マップのサイズ
#define	MAP_W		(30)
#define	MAP_H		(17)

// ステージデータ数
#define	STAGE_MAX	(4)

// 手の保存（戻せる）数
#define	STEPSAVE_NUM	(21)

#define MAP_ROW ((SCREEN_H / CHIP_H) + 1)	// マップの行数(小数点以下は切り捨てのため+1する)
#define MAP_COL (SCREEN_W / CHIP_W)	// マップの列数

//4方向定義
#define	ARROW_DN	(0)	// 下
#define	ARROW_LF	(1)	// 左
#define	ARROW_RI	(2)	// 右
#define	ARROW_UP	(3)	// 上

//シナリオシーンの種類
#define SCENARIO_NUMBER			(22)	// シナリオのカットシーンの数

//クリア後シナリオの種類
#define SCENARIO_CLEAR_NUMBER	(11)		// クリア後のシナリオのカットシーンの数

struct CHARACTER {
	int w, h;		// 大きさ
	int	speed;		// 移動速度
	int	animcnt;	// アニメーションカウンタ
	int animspeed;	// アニメーション速度
	int	arrow;		// 向いている方向
	int animTbl[]; 	// アニメーション順の指定
};




//
// グローバル変数宣言
//

// マップデータ
int	map[MAP_W * MAP_H];
int box[MAP_W * MAP_H];
int item[MAP_W * MAP_H];	// アイテム（回復ドット）データ

// 手の保存データ（マップ＋プレイヤー位置）
int stepsave_map[STEPSAVE_NUM][MAP_W * MAP_H];
int stepsave_box[STEPSAVE_NUM][MAP_W * MAP_H];
int stepsave_playerX[STEPSAVE_NUM];
int stepsave_playerY[STEPSAVE_NUM];
int stepsave_cnt;	// 手の保存データカウンタ

int phase = 0;		// ゲームフェーズ(0:タイトル、1:ゲーム中、2:シナリオ、3:ゲームクリア、4:ステージクリア、5:ゲームオーバー)
bool isGameEnd = false; // ゲーム終了フラグ（true:ゲーム終了、false:ゲーム継続中）

// ステージデータ(mapとboxとplayer位置を混ぜたもの)
// // 0:床, 1:壁, 2:食べ物(弱), 3:食べ物(中), 4:食べ物(強), 5:箱(弱), 6:箱(中), 7:箱(強), 8透明(base),  9:player, 10:ゴール1, 11:ゴール2, 12:ゴール3, 13:ゴール4, 14最終ゴール, 15最終ゴール2, 16最終ゴール3, 17最終ゴール4, 18食べ物(最強)
int stage[STAGE_MAX][MAP_W * MAP_H] = {
	{	// stage 0
		8,8,8,8,8,8,1,1,1,1,1,1,1,1,10,11,1,1,1,1,1,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,1,1,1,1,1,12,13,1,1,1,1,1,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,1,1,1,1,1,0,3,1,0,0,2,0,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,1,1,1,1,1,0,2,1,0,0,1,2,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,1,1,1,1,1,0,2,1,0,0,0,0,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,1,1,1,1,1,5,1,1,0,0,0,0,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,0,0,0,0,2,5,0,1,0,0,0,0,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,2,1,0,0,1,1,0,1,0,1,0,0,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,0,0,0,0,2,1,1,1,0,2,0,0,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,0,0,0,0,1,0,0,5,0,0,0,0,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,0,0,0,0,0,0,2,1,0,0,0,0,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,0,0,0,1,1,0,0,1,0,0,0,0,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,0,1,2,1,2,1,0,1,0,0,0,0,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,0,2,0,1,0,2,0,1,0,9,0,1,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	},
	// stage 1
	{
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,10,11,1,8,8,8,8,8,8,
		8,8,8,8,1,1,0,0,2,0,0,0,1,1,1,1,2,0,0,0,1,12,13,1,8,8,8,8,8,8,
		8,8,8,8,1,2,1,0,1,2,0,5,5,0,1,1,0,0,0,2,1,0,0,1,8,8,8,8,8,8,
		8,8,8,8,1,0,0,0,0,0,1,0,5,0,1,1,2,0,0,6,0,0,3,1,8,8,8,8,8,8,
		8,8,8,8,1,2,0,2,0,0,1,0,0,1,2,0,0,0,5,0,5,0,0,1,8,8,8,8,8,8,
		8,8,8,8,1,1,5,1,0,0,1,0,0,1,1,1,1,1,0,0,1,5,1,1,8,8,8,8,8,8,
		8,8,8,8,1,0,0,1,0,0,1,1,2,1,1,0,0,0,0,0,0,0,0,1,8,8,8,8,8,8,
		8,8,8,8,1,0,1,1,1,1,1,0,0,1,0,0,1,0,0,0,1,1,1,1,8,8,8,8,8,8,
		8,8,8,8,1,0,1,2,0,0,1,0,5,1,0,1,0,0,0,0,5,0,0,1,8,8,8,8,8,8,
		8,8,8,8,1,0,0,0,0,0,1,5,0,2,0,2,0,0,0,0,1,0,2,1,8,8,8,8,8,8,
		8,8,8,8,1,9,1,2,0,2,1,0,0,0,0,0,0,0,0,0,1,3,1,1,8,8,8,8,8,8,
		8,8,8,8,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	},
	// stage 2
	{
		8,1,10,11,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,8,8,8,
		8,1,12,13,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,8,8,8,8,8,
		8,1,0,0,1,2,0,0,0,0,0,1,1,1,2,1,0,0,6,0,1,1,1,1,1,8,8,8,8,8,
		8,1,0,0,1,0,0,2,1,0,0,1,0,0,5,2,1,0,0,5,2,0,1,2,1,8,8,8,8,8,
		8,1,0,0,0,0,0,0,2,6,0,0,0,6,0,0,5,0,0,0,5,0,1,0,1,8,8,8,8,8,
		8,1,1,6,1,1,0,0,0,0,0,1,0,5,0,0,0,1,1,0,1,0,0,0,1,8,8,8,8,8,
		8,1,1,0,4,0,6,1,1,1,1,1,1,1,1,1,1,1,1,5,1,0,0,0,1,8,8,8,8,8,
		8,1,1,1,1,1,0,1,0,0,0,0,0,0,5,0,1,2,0,0,1,0,0,1,1,8,8,8,8,8,
		8,1,1,1,1,0,0,1,0,0,0,0,0,3,5,0,0,1,0,0,6,0,2,1,1,8,8,8,8,8,
		8,1,1,1,0,0,0,0,0,0,0,0,1,1,0,1,2,1,1,0,1,1,0,1,1,8,8,8,8,8,
		8,1,1,1,5,5,0,1,2,0,0,0,0,2,1,1,0,0,0,6,2,0,0,1,1,8,8,8,8,8,
		8,1,1,0,0,0,1,1,1,1,1,1,1,1,1,2,0,0,0,0,1,1,1,1,1,8,8,8,8,8,
		8,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,8,8,8,8,
		8,1,0,3,2,1,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,1,0,9,0,1,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,1,1,1,1,1,1,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	},

	// stage 3
	{
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,8,8,8,8,
		1,0,0,1,0,6,1,0,0,1,0,0,6,0,0,0,1,2,1,3,0,0,2,7,1,8,8,8,8,8,
		1,0,0,0,6,0,0,0,0,6,0,0,0,6,0,7,0,0,0,7,0,0,0,2,1,8,8,8,8,8,
		1,0,2,0,6,0,6,2,0,7,0,0,6,7,0,0,6,2,7,0,2,0,0,0,1,8,8,8,8,8,
		1,0,6,0,6,0,1,14,15,1,1,0,1,1,1,1,1,1,1,1,0,6,0,0,1,8,8,8,8,8,
		1,0,0,1,0,0,1,16,17,0,0,7,1,0,0,0,0,0,0,1,1,1,5,1,1,8,8,8,8,8,
		1,0,0,2,6,7,0,18,18,0,1,0,0,0,1,18,7,7,0,1,0,0,0,0,1,8,8,8,8,8,
		1,0,0,0,0,1,2,0,0,0,1,1,1,5,0,7,1,0,0,7,0,0,0,0,1,8,8,8,8,8,
		1,0,0,2,0,1,4,3,0,6,0,0,0,0,0,7,0,0,7,0,5,0,0,0,1,8,8,8,8,8,
		1,1,5,6,5,1,1,1,1,1,1,7,1,0,2,1,7,1,2,0,5,0,0,0,1,8,8,8,8,8,
		1,0,0,0,0,1,1,0,0,6,0,0,7,6,7,7,0,3,5,5,0,6,1,0,1,8,8,8,8,8,
		1,0,0,0,0,1,1,7,0,7,0,0,6,0,0,0,7,5,0,0,0,3,1,0,1,8,8,8,8,8,
		1,0,6,0,0,7,0,0,0,0,6,6,0,0,0,0,0,0,6,0,0,1,0,0,1,8,8,8,8,8,
		1,0,0,7,0,0,1,1,0,6,0,0,0,0,0,0,4,7,2,6,5,1,3,2,1,8,8,8,8,8,
		1,0,3,1,4,0,1,1,0,7,1,0,1,0,0,0,0,1,5,9,3,1,7,7,1,8,8,8,8,8,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,8,8,8,8,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,8,
	},
};


// 画像データ
int cgChip[7];					// チップ画像（配列）
int cgPlayer[4][3 * 4];			// プレイヤー
int playerDrowX, playerDrowY;	// プレイヤーの描画位置（チップ単位）
int cgBox[3];					// 箱
int cgMap;						// マップチップ
int cggoal[4][2 * 2];			// ゴール2
int operation;					// 操作用画像（ボタンなど）
int energy_ui_slime;			// エネルギーUI（外側）
int energy_ui_textbox;			// エネルギーUI（内側）
int energy_ui_number[10];		// エネルギーUI（数字）
int energy_ui_number_Hpmax[2];	// エネルギーUI（HP最大）画像
int next_image;			// 次に進む画像

int  title_image;											// タイトル画像
int  scenario_text_image[SCENARIO_NUMBER];					// シナリオテキスト画像
int  scenario_clear_text_image[SCENARIO_CLEAR_NUMBER - 3];	// クリア後のシナリオテキスト画像
int  stage_clear_image;										// ステージクリア画像
int  game_clear_image;										// ゲームクリア画像
int  game_over_image;										// ゲームオーバー画像
int  animTbl[] = { 0,1,2,1 };								// アニメーション順の指定

//シナリオキャラ
int  scenario_slime[8];		// シナリオスライム
int  scenario_girl[5];		// シナリオガール




// ゲーム情報
int gKey;			// キー入力の情報
int gTrg;			// トリガ入力の情報
int gStageNo;		// ステージ番号
int gViewClearCnt;	// ステージクリア表示カウンタ
int scenarioCnt;	// シナリオカウンタ
int clear_scenario;  //クリア後のシナリオカウント

// プレイヤー情報
int playerX, playerY;		// 座標（チップ単位）
struct PlayerStatus plHP;	// プレイヤーのステータス（HPなど）
struct CHARACTER chara;		// キャラクター情報（プレイヤー）
int touch;					//0:タッチしていない 1:タッチしている


// ステージデータ初期化
void StageInit(int stageNo) {
	// stage[stageNo]から、map[], box[], player座標を生成する
	int x, y;
	for (y = 0; y < MAP_H; y++) {
		for (x = 0; x < MAP_W; x++) {

			// stage[stageNo][] から、チップ番号を取り出す
			// 0:床, 1:壁, 2:食べ物(弱), 3:食べ物(中), 4:食べ物(強), 5:箱(弱), 6:箱(中), 7:箱(強), 8透明(base),  9:player, 10:ゴール1, 11:ゴール2, 12:ゴール3, 13:ゴール4, 14最終ゴール, 15最終ゴール2, 16最終ゴール3, 17最終ゴール4, 18食べ物(最強)
			int chip_no = stage[stageNo][y * MAP_W + x];

			// box[]の書き込み
			box[y * MAP_W + x] = 0;
			if (chip_no == 5) { box[y * MAP_W + x] = 5; } // 箱(弱)
			if (chip_no == 6) { box[y * MAP_W + x] = 6; } // 箱(中)
			if (chip_no == 7) { box[y * MAP_W + x] = 7; } // 箱(強)

			//item[]の書き込み
			item[y * MAP_W + x] = 0; // アイテムは初期化
			if (chip_no == 2) { item[y * MAP_W + x] = 2; }	// 食べ物（弱）
			if (chip_no == 3) { item[y * MAP_W + x] = 3; }	// 食べ物（中）
			if (chip_no == 4) { item[y * MAP_W + x] = 4; }	// 食べ物（強）
			if (chip_no == 18) { item[y * MAP_W + x] = 18; }// 食べ物（最強）

			map[y * MAP_W + x] = 0;
			// map[]の書き込み
			if (chip_no == 0) { map[y * MAP_W + x] = 0; }	// 床
			if (chip_no == 1) { map[y * MAP_W + x] = 1; }	// 壁
			if (chip_no == 2) { item[y * MAP_W + x] = 2; }	// 食べ物（弱）
			if (chip_no == 3) { item[y * MAP_W + x] = 3; }	// 食べ物（中）
			if (chip_no == 4) { item[y * MAP_W + x] = 4; }	// 食べ物（強）
			if (chip_no == 5) { box[y * MAP_W + x] = 5; }	// 箱(弱)
			if (chip_no == 6) { box[y * MAP_W + x] = 6; }	// 箱(中)
			if (chip_no == 7) { box[y * MAP_W + x] = 7; }	// 箱(強)
			if (chip_no == 8) { map[y * MAP_W + x] = 8; }	//透明（床）

			if (chip_no == 9) {								// プレイヤー
				// ここにプレイヤー座標を設定
				map[y * MAP_W + x] = 0;
				playerX = x;
				playerY = y;
				playerDrowX = playerX * CHIP_W;					// プレイヤーの描画位置（チップ単位）
				playerDrowY = playerY * CHIP_H;					// プレイヤーの描画位置（チップ単位）
				plHP.current_hp = 10;							// プレイヤーのHP初期値
				plHP.max_hp = 40;								// プレイヤーの最大HP
				plHP.hp_step = 10;								// 画像を切り替えるHPの間隔
				plHP.num_img = plHP.max_hp / plHP.hp_step + 1;	// 画像の枚数
			}

			// ゴールの設定
			if (chip_no == 10) { map[y * MAP_W + x] = 10; }		// ゴール1
			if (chip_no == 11) { map[y * MAP_W + x] = 11; }		// ゴール2
			if (chip_no == 12) { map[y * MAP_W + x] = 12; }		// ゴール3
			if (chip_no == 13) { map[y * MAP_W + x] = 13; }		// ゴール4
			if (chip_no == 14) { map[y * MAP_W + x] = 14; }		// 最終ゴール
			if (chip_no == 15) { map[y * MAP_W + x] = 15; }		// 最終ゴール2
			if (chip_no == 16) { map[y * MAP_W + x] = 16; }		// 最終ゴール3
			if (chip_no == 17) { map[y * MAP_W + x] = 17; }		// 最終ゴール4
			if (chip_no == 18) { item[y * MAP_W + x] = 18; }	// 食べ物（最強）


		}
	}
	gViewClearCnt = 0;

	// 保存カウンタ
	stepsave_cnt = 0;

	touch = 0; // タッチしていない状態にする
}

//// 1手保存する関数
//void SaveOneStep() {
//
//	// stepsave_*****[0?] → [1?]にコピー
//	// iは後ろから回さないといけない
//	for (int i = STEPSAVE_NUM - 2; i >= 0; i--) {
//		// map, box
//		for (int n = 0; n < MAP_W * MAP_H; n++) {
//			stepsave_map[i + 1][n] = stepsave_map[i][n];
//			stepsave_box[i + 1][n] = stepsave_box[i][n];
//		}
//		// playerX,Y
//		stepsave_playerX[i + 1] = stepsave_playerX[i];
//		stepsave_playerY[i + 1] = stepsave_playerY[i];
//	}
//
//	// stepsave_*****[0] に、現状を保存
//	for (int n = 0; n < MAP_W * MAP_H; n++) {
//		stepsave_map[0][n] = map[n];
//		stepsave_box[0][n] = box[n];
//	}
//	stepsave_playerX[0] = playerX;
//	stepsave_playerY[0] = playerY;
//
//	// 手の保存データカウンタを増やす
//	stepsave_cnt++;
//	if (stepsave_cnt > STEPSAVE_NUM) { stepsave_cnt = STEPSAVE_NUM; }
//}

// 1手戻す関数
//void BackOneStep() {
//	// 手の保存データカウンタは1以上あるか？
//	if (stepsave_cnt > 0) {
//		// stepsave_*****[0] を、現状にコピー
//		for (int n = 0; n < MAP_W * MAP_H; n++) {
//			map[n] = stepsave_map[0][n];
//			box[n] = stepsave_box[0][n];
//		}
//		playerX = stepsave_playerX[0];
//		playerY = stepsave_playerY[0];
//
//		// stepsave_*****[1?] → [0?]にコピー
//		for (int i = 0; i < STEPSAVE_NUM - 1; i++) {
//			// map, box
//			for (int n = 0; n < MAP_W * MAP_H; n++) {
//				stepsave_map[i][n] = stepsave_map[i + 1][n];
//				stepsave_box[i][n] = stepsave_box[i + 1][n];
//			}
//			// playerX,Y
//			stepsave_playerX[i] = stepsave_playerX[i + 1];
//			stepsave_playerY[i] = stepsave_playerY[i + 1];
//		}
//
//		// 手の保存データカウンタを減らす
//		stepsave_cnt--;
//	}
//}



// アプリの初期化
// 起動時に1回だけ実行される
void AppInit() {
	// 画像の読み込み
	cgChip[0] = LoadGraph("res/floor_04.png");		//透明チップの変わりにベースチップを使う
	cgChip[1] = LoadGraph("res/tree_03.png");	//壁（森)
	cgChip[2] = LoadGraph("res/kinomi.png");	//食べ物（弱）
	cgChip[3] = LoadGraph("res/cookie.png");	//食べ物（中）
	cgChip[4] = LoadGraph("res/cupcake.png");	//食べ物（強）
	cgChip[5] = LoadGraph("res/base.png");  //床
	cgChip[6] = LoadGraph("res/candy.png"); //食べ物（最強）

	chara.w = CHIP_W;		// キャラクターの幅
	chara.h = CHIP_H;		// キャラクターの高さ
	chara.speed = 1;		// キャラクターの移動速度
	chara.animcnt = 0;		// アニメーションカウンタ
	chara.animspeed = 8;	// アニメーション速度
	chara.arrow = ARROW_DN;	// 向いている方向（下向き）


	cgBox[0] = LoadGraph("res/rock_01.png");			//箱(中)
	cgBox[1] = LoadGraph("res/tree_branch_01.png");	//箱(弱)
	cgBox[2] = LoadGraph("res/barrel_01.png");			//箱3(強)

	cgMap = LoadGraph("res/Stage_01_01.png");//背景
	LoadDivGraph("res/goal1_03.png", 2 * 2, 2, 2, 64, 64, cggoal[0]);//ゴール1
	LoadDivGraph("res/goal1_03.png", 2 * 2, 2, 2, 64, 64, cggoal[1]);//ゴール1
	LoadDivGraph("res/goal2_02.png", 2 * 2, 2, 2, 64, 64, cggoal[2]);//ゴール2
	LoadDivGraph("res/goal3.png", 2 * 2, 2, 2, 64, 64, cggoal[3]);//ゴール3

	LoadDivGraph("res/slyme_small_02.png", 3 * 4, 3, 4, 64, 64, cgPlayer[0]);	// スライム小
	LoadDivGraph("res/slime_middle.png", 3 * 4, 3, 4, 64, 64, cgPlayer[1]);	// スライム中
	LoadDivGraph("res/slime_big.png", 3 * 4, 3, 4, 64, 64, cgPlayer[2]);	// スライム大
	LoadDivGraph("res/slime_hinnsi.png", 3 * 4, 3, 4, 64, 64, cgPlayer[3]);	// 瀕死スライム

	operation = LoadGraph("res/operation_screen2.png");			// 操作用画像（ボタンなど）
	energy_ui_slime = LoadGraph("res/slime_ui_outside_02.png");	// エネルギーUI（外側）
	energy_ui_textbox = LoadGraph("res/slime_ui_inside_02.png");	// エネルギーUI（内側）
	for (int i = 0; i < 10; i++) {								// エネルギーUI（数字）
		char filename[999];
		sprintf(filename, "res/number_%d.png", i);
		energy_ui_number[i] = LoadGraph(filename);
	}
	energy_ui_number_Hpmax[0] = LoadGraph("res/number_0_red.png");	// エネルギーUI（HP最大）
	energy_ui_number_Hpmax[1] = LoadGraph("res/number_4_red.png");	// エネルギーUI（HP最大2）

	//タイトル
	title_image = LoadGraph("res/Title.png");			// タイトル画像

	//シナリオ
	for (int i = 0; i < SCENARIO_NUMBER; i++) {								// エネルギーUI（数字）
		char filename[999];
		sprintf(filename, "res/textbox_%d.png", i + 1);
		scenario_text_image[i] = LoadGraph(filename);
	}

	//ゲームクリアシナリオ
	for (int i = 0; i < SCENARIO_CLEAR_NUMBER - 3; i++) {								// エネルギーUI（数字）
		char filename[999];
		sprintf(filename, "res/textbox_clear_%d.png", i + 1);
		scenario_clear_text_image[i] = LoadGraph(filename);
	}

	//スライム立ち絵
	for (int i = 0; i < 8; i++) {
		char filename[999];
		sprintf(filename, "res/slime_%d.png", i + 1);
		scenario_slime[i] = LoadGraph(filename);
	}

	//女の子立ち絵
	for (int i = 0; i < 5; i++) {
		char filename[999];
		sprintf(filename, "res/girl_%d.png", i + 1);
		scenario_girl[i] = LoadGraph(filename);
	}

	//ゲーム状態（クリアorゲームオーバー)
	stage_clear_image = LoadGraph("res/stageclear!_02.png");	// ステージクリア画像
	game_clear_image = LoadGraph("res/gameclear!!_02.png");		// ゲームクリア画像
	game_over_image = LoadGraph("res/game_over.png");			// ゲームオーバー画像

	//次に進む画像
	next_image = LoadGraph("res/space_next.png");					// 次に進む画像



	// ステージデータ初期化
	gStageNo = 0;
	StageInit(gStageNo);

	//シナリオカウンタ初期化
	scenarioCnt = 0;
	clear_scenario = 0;
}


// アプリの解放
// 終了時に1回だけ実行される
void AppRelease() {
}





//
// フレーム処理。1フレームに1回ずつ実行される
//

// フレーム処理：入力
void FrameInput() {
	// キーの入力、トリガ入力を得る
	int keyold = gKey;
	gKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);		// キー入力を取得
	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）
	gTrg = (gKey ^ keyold) & gKey;
}

// フレーム処理：計算
void FrameProcess() {
	if (phase == 0) {//タイトルフェーズ
		if (gTrg & PAD_INPUT_10)
		{
			phase = 1;
		}
		else
		{
			//何も処理しない
		}
		return; // タイトル中は他の処理をしない
	}

	if (phase == 1) {//シナリオフェーズ
		if (gTrg & PAD_INPUT_9) {
			//phase = 2; //開発者用シナリオ終了フェーズへ移行
		}
		else
		{
			if (gTrg & PAD_INPUT_10)
			{
				if (scenarioCnt < SCENARIO_NUMBER - 1) {
					scenarioCnt++;	// シナリオカウンタを進める
				}
				else
				{
					scenarioCnt = 0;	// シナリオカウンタをリセット
					phase = 2;	// シナリオ終了フェーズへ移行
				}
			}
			else
			{
				//何も処理しない
			}
		}
		return; // シナリオ中は他の処理をしない
	}

	if (phase == 3) {//ゲームクリアフェーズ
		if (gTrg & PAD_INPUT_10)
		{
			if (clear_scenario < SCENARIO_CLEAR_NUMBER - 1) {
				clear_scenario++;	// シナリオカウンタを進める
			}
			else
			{
				isGameEnd = true;
			}
		}
		else
		{
			//何も処理しない
		}
		return; // シナリオ中は他の処理をしない
	}

	if (phase == 4) {//ゲームクリアフェース
		return; // タイトル中は他の処理をしない
	}


	if (phase == 5) {//ゲームオーバーフェース
		return; // タイトル中は他の処理をしない
	}


	if (playerX * CHIP_W != playerDrowX || playerY * CHIP_H != playerDrowY) {
		int speed = 4;	// キャラクターの移動速度
		if (playerX * CHIP_W < playerDrowX) { playerDrowX -= speed; }	// 左に移動
		if (playerX * CHIP_W > playerDrowX) { playerDrowX += speed; }	// 右に移動
		if (playerY * CHIP_H < playerDrowY) { playerDrowY -= speed; }	// 上に移動
		if (playerY * CHIP_H > playerDrowY) { playerDrowY += speed; }	// 下に移動
	}
	else {


		// 移動前の場所を取っておく
		int old_x = playerX;
		int old_y = playerY;

		// 移動方向用変数
		int move_x = 0;
		int move_y = 0;

		//// 何か方向キーを押したら今の位置を保存
		//if (gTrg & PAD_INPUT_LEFT + PAD_INPUT_RIGHT + PAD_INPUT_UP + PAD_INPUT_DOWN) {
		//	SaveOneStep();
		//}

		// キー入力を判定して、主人公を移動させる
		if (gTrg & PAD_INPUT_LEFT) { playerX -= chara.speed; move_x = -1; chara.arrow = ARROW_LF; }
		if (gTrg & PAD_INPUT_RIGHT) { playerX += chara.speed; move_x = 1; chara.arrow = ARROW_RI; }
		if (move_x == 0) {	// 斜め移動しないように、横移動を優先
			if (gTrg & PAD_INPUT_UP) { playerY -= chara.speed; move_y = -1; chara.arrow = ARROW_UP; }
			if (gTrg & PAD_INPUT_DOWN) { playerY += chara.speed; move_y = 1; chara.arrow = ARROW_DN; }
			chara.animcnt++;	// アニメーションカウンタを進める
		}

		if (playerX != old_x || playerY != old_y) {
			if (map[playerY * MAP_W + playerX] == 0) {
				plHP.current_hp -= 1;	// 移動したのでHPを1減らす
			}
			if (plHP.current_hp < 0) {

				plHP.current_hp = 0;	// 最低値を0にする
			}
		}
		else {
			plHP.current_hp -= 0;	// 移動しなかったのでHPを減らさない
		}

		//回復
		if (item[playerY * MAP_W + playerX] == 2) {
			if (plHP.current_hp < plHP.max_hp) {
				// ドットの上に立ったので、HPを回復する
				int recovery_amount = 6;
				plHP.current_hp += recovery_amount;


				////開発者デバック
				//DrawFormatString(0, 0, GetColor(255, 255, 255), "HP回復: %d", recovery_amount);
				//ScreenFlip();

				if (plHP.current_hp > plHP.max_hp) {
					plHP.current_hp = plHP.max_hp;	// 最大値を超えないようにする
				}
			}

			//map[playerY * MAP_W + playerX] = 0;	// ドットを消す
			item[playerY * MAP_W + playerX] = 0;	// ドットを消す
		}

		//回復
		if (item[playerY * MAP_W + playerX] == 3) {
			if (plHP.current_hp < plHP.max_hp) {
				// ドットの上に立ったので、HPを回復する
				int recovery_amount = 11;
				plHP.current_hp += recovery_amount;


				////開発者デバック
				//DrawFormatString(0, 0, GetColor(255, 255, 255), "HP回復: %d", recovery_amount);
				//ScreenFlip();

				if (plHP.current_hp > plHP.max_hp) {
					plHP.current_hp = plHP.max_hp;	// 最大値を超えないようにする
				}
			}

			item[playerY * MAP_W + playerX] = 0;	// ドットを消す
		}

		if (item[playerY * MAP_W + playerX] == 4) {
			if (plHP.current_hp < plHP.max_hp) {
				// ドットの上に立ったので、HPを回復する
				int recovery_amount = 21;
				plHP.current_hp += recovery_amount;

				////開発者デバック
				//DrawFormatString(0, 0, GetColor(255, 255, 255), "HP回復: %d", recovery_amount);
				//ScreenFlip();

				if (plHP.current_hp > plHP.max_hp) {
					plHP.current_hp = plHP.max_hp;	// 最大値を超えないようにする
				}
			}

			item[playerY * MAP_W + playerX] = 0;	// ドットを消す
		}


		if (item[playerY * MAP_W + playerX] == 18) {
			if (plHP.current_hp < plHP.max_hp) {
				// ドットの上に立ったので、HPを回復する
				int recovery_amount = 41;
				plHP.current_hp += recovery_amount;

				////開発者用デバック
				//DrawFormatString(0, 0, GetColor(255, 255, 255), "HP回復: %d", recovery_amount);
				//ScreenFlip();

				if (plHP.current_hp > plHP.max_hp) {
					plHP.current_hp = plHP.max_hp;	// 最大値を超えないようにする
				}
			}

			item[playerY * MAP_W + playerX] = 0;	// ドットを消す
		}

		// 移動した先が壁か？
		if (map[playerY * MAP_W + playerX] == 1)
		{
			// 移動した先が壁だったので、移動前の場所に戻す
			playerX = old_x;
			playerY = old_y;

		}

		if (plHP.current_hp <= 40 && plHP.current_hp >= 29) {
			// さらに同じ方向のその先の座標を求める（移動方向用変数を足す）
			int b = box[playerY * MAP_W + playerX];
			// 移動した先に箱はあるか？
			//if (box[playerY * MAP_W + playerX] == 5 || box[playerX * MAP_W + playerX] == 6 || box[playerX * MAP_W + playerX] == 7)
			if (b == 5 || b == 6 || b == 7)
			{
				// 移動した先に箱があった。
				// 押せるかどうか調べたい
				int push_ok = 1;

				// さらに同じ方向のその先の座標を求める（移動方向用変数を足す）
				int next_x = playerX + move_x;
				int next_y = playerY + move_y;


				// その先に、壁があるかを調べる
				if (map[next_y * MAP_W + next_x] == 1)
				{
					// 壁があるので押せない
					push_ok = 0;
					plHP.current_hp += 1;	// 押したのでHPを1減らす
				}

				// その先に、箱があるかを調べる
				if (box[next_y * MAP_W + next_x] == 5 || box[next_y * MAP_W + next_x] == 6 || box[next_y * MAP_W + next_x] == 7)

				{
					// 箱があるので押せない
					push_ok = 0;
					plHP.current_hp += 1;	// 押したのでHPを1減らす
				}
				if (item[next_y * MAP_W + next_x] == 2 || item[next_y * MAP_W + next_x] == 3 || item[next_y * MAP_W + next_x] == 4 || item[next_y * MAP_W + next_x] == 18) {
					// アイテムがあるので押せない
					push_ok = 0;
					plHP.current_hp += 1;	// 押したのでHPを1減らす
				}

				// 調べた結果、押せる？
				if (push_ok == 1)
				{
					if (plHP.current_hp >= 2) {
						// HPが2以上なら、押せる
						plHP.current_hp -= 1;	// 押したのでHPを1減らす
					}
					else {
						// HPが1以下なら、押せない
						push_ok = 0;
					}

					// 押せる。箱の位置を移動する
					// まず、プレイヤーが乗った箱を消し、
					box[playerY * MAP_W + playerX] = 0;

					// 移動先に箱を書き込む
					box[next_y * MAP_W + next_x] = b;
				}
				else
				{
					// 押せないので、プレイヤーの位置を移動前の場所に戻す
					playerX = old_x;
					playerY = old_y;
				}
			}
		}

		if (plHP.current_hp < 29 && plHP.current_hp >= 19 || plHP.current_hp >= 30) {
			int b = box[playerY * MAP_W + playerX];
			// 移動した先に箱はあるか？
			if (b == 5 || b == 6 || b == 7)
			{
				if (b == 7) {
					playerX = old_x;
					playerY = old_y;
					plHP.current_hp += 1;	// 押したのでHPを1減らす
				}
				else if (b == 5 || b == 6) {
					int push_ok = 1;
					// さらに同じ方向のその先の座標を求める（移動方向用変数を足す）
					int next_x = playerX + move_x;
					int next_y = playerY + move_y;

					// その先に、壁があるかを調べる
					if (map[next_y * MAP_W + next_x] == 1)
					{
						// 壁があるので押せない
						push_ok = 0;
						plHP.current_hp += 1;	// 押したのでHPを1減らす
					}

					// その先に、箱があるかを調べる
					int a = box[next_y * MAP_W + next_x];
					if (a == 5 || a == 6 || a == 7)
					{
						// 箱があるので押せない
						push_ok = 0;
						//playerX = old_x;
						//playerY = old_y;
						plHP.current_hp += 1;	// 押したのでHPを1減らす
					}
					if (item[next_y * MAP_W + next_x] == 2 || item[next_y * MAP_W + next_x] == 3 || item[next_y * MAP_W + next_x] == 4 || item[next_y * MAP_W + next_x] == 18) {
						// アイテムがあるので押せない
						push_ok = 0;
						plHP.current_hp += 1;	// 押したのでHPを1減らす
					}

					// 調べた結果、押せる？
					if (push_ok == 1)
					{
						if (plHP.current_hp >= 2) {
							// HPが2以上なら、押せる
							plHP.current_hp -= 1;	// 押したのでHPを1減らす
						}
						else {
							// 

							push_ok = 0;

						}

						// 押せる。箱の位置を移動する
						// まず、プレイヤーが乗った箱を消し、
						box[playerY * MAP_W + playerX] = 0;
						// 移動先に箱を書き込む
						//if (a == 6 || a == 7) {
						box[next_y * MAP_W + next_x] = b;
						//}
					}
					else
					{
						// 押せないので、プレイヤーの位置を移動前の場所に戻す

						playerX = old_x;
						playerY = old_y;

					}
				}
			}
		}

		if (plHP.current_hp <= 19 || plHP.current_hp >= 20) {
			int b = box[playerY * MAP_W + playerX];
			// 移動した先に箱はあるか？
			if (b == 5 || b == 6 || b == 7)
			{
				if (b == 6 || b == 7) {
					playerX = old_x;
					playerY = old_y;
					plHP.current_hp += 1;	// 押したのでHPを1減らす
				}
				else if (b == 5) {
					int push_ok = 1;
					// さらに同じ方向のその先の座標を求める（移動方向用変数を足す）
					int next_x = playerX + move_x;
					int next_y = playerY + move_y;


					// その先に、壁があるかを調べる
					if (map[next_y * MAP_W + next_x] == 1)
					{
						// 壁があるので押せない
						push_ok = 0;
						plHP.current_hp += 1;	// 押したのでHPを1減らす
					}

					// その先に、箱があるかを調べる
					int a = box[next_y * MAP_W + next_x];
					if (a == 5 || a == 6 || a == 7)
					{
						// 箱があるので押せない
						push_ok = 0;
						playerX = old_x;
						playerY = old_y;
						plHP.current_hp += 1;	// 押したのでHPを1減らす
					}
					if (item[next_y * MAP_W + next_x] == 2 || item[next_y * MAP_W + next_x] == 3 || item[next_y * MAP_W + next_x] == 4 || item[next_y * MAP_W + next_x] == 18) {
						// アイテムがあるので押せない
						push_ok = 0;
						plHP.current_hp += 1;	// 押したのでHPを1減らす
					}

					// 調べた結果、押せる？
					if (push_ok == 1)
					{
						if (plHP.current_hp >= 2) {
							// HPが2以上なら、押せる
							plHP.current_hp -= 1;	// 押したのでHPを1減らす
						}
						else {
							// 

							push_ok = 0;

						}

						// 押せる。箱の位置を移動する
						// まず、プレイヤーが乗った箱を消し、
						box[playerY * MAP_W + playerX] = 0;
						// 移動先に箱を書き込む
						//if (a == 5)
						box[next_y * MAP_W + next_x] = b;
						//}
					}
					else
					{
						// 押せないので、プレイヤーの位置を移動前の場所に戻す

						playerX = old_x;
						playerY = old_y;

					}
				}
			}
		}

		if (plHP.current_hp >= 20) {
			//プレイヤーがゴールにいるか？
			if (map[playerY * MAP_W + playerX] == 10 ||
				map[playerY * MAP_W + playerX] == 11 ||
				map[playerY * MAP_W + playerX] == 12 ||
				map[playerY * MAP_W + playerX] == 13) {
				//次のステージへ
				if (gStageNo + 1 < STAGE_MAX) {
					phase = 4;	// クリアフェーズへ
				}
			}

		}
		else if (plHP.current_hp < 20) {
			if (map[playerY * MAP_W + playerX] == 10 ||
				map[playerY * MAP_W + playerX] == 11 ||
				map[playerY * MAP_W + playerX] == 12 ||
				map[playerY * MAP_W + playerX] == 13) {

				playerX = old_x;	// プレイヤーの位置を戻す
				playerY = old_y;	// プレイヤーの位置を戻す
			}

		}

		if (plHP.current_hp > 30) {
			//プレイヤーが最終ゴールにいるか？
			if (map[playerY * MAP_W + playerX] == 14 ||
				map[playerY * MAP_W + playerX] == 15 ||
				map[playerY * MAP_W + playerX] == 16 ||
				map[playerY * MAP_W + playerX] == 17) {
				//ゴールしたのでクリアステージを表示する
				//次のステージへ
				////if (gStageNo + 1 < STAGE_MAX) {
				////	gStageNo++;
				////	StageInit(gStageNo);
				////}
				//else {
					// 全ステージクリアしたので、ゲームクリア
				phase = 3;	// クリアフェーズへ
				//}
			}
		}
		else if (plHP.current_hp <= 30) {
			if (map[playerY * MAP_W + playerX] == 14 ||
				map[playerY * MAP_W + playerX] == 15 ||
				map[playerY * MAP_W + playerX] == 16 ||
				map[playerY * MAP_W + playerX] == 17) {

				playerX = old_x;	// プレイヤーの位置を戻す
				playerY = old_y;	// プレイヤーの位置を戻す
			}
		}
	}

	if (plHP.current_hp <= 0) {// HPが0以下になったので、ゲームオーバー
		phase = 5;	// ゲームオーバーフェーズへ
	}

	// ステージデータリセット（やり直し）
	if (gTrg & PAD_INPUT_1) {	// [z]
		StageInit(gStageNo);
	}

	//// 開発者用１歩戻す
	//if (gTrg & PAD_INPUT_3) {	// [c]
	//	BackOneStep();
	//}

	// 開発用。次のステージへ
	if (gTrg & PAD_INPUT_2) {	// [x]
		if (gStageNo + 1 < STAGE_MAX) {
			gStageNo++;
			StageInit(gStageNo);
		}
	}

}


// フレーム処理：描画
void FrameDraw() {

	ClearDrawScreen();						// 画面を初期化する

	int left_up = 0, right_up = 0, left_down = 0, right_down = 0, playerImgIdx = 0;

	switch (phase) // フェーズによって描画を変える
	{
	case 0:	// タイトル画面
		DrawGraph(0, 0, title_image, TRUE);
		DrawGraph(1600, 800, next_image, TRUE);
		break;

	case 1:	// シナリオ画面
		// scenariocntが1～29のとき、それぞれ対応するscenario_text_image[]を描画
		switch (scenarioCnt) {
		case 0:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(0, 0, scenario_text_image[0], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 1:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[1], TRUE);
			DrawGraph(0, 0, scenario_text_image[1], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 2:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[1], TRUE);
			DrawGraph(0, 0, scenario_text_image[2], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 3:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_text_image[3], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 4:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_text_image[4], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 5:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_text_image[5], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 6:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[3], TRUE);
			DrawGraph(0, 0, scenario_text_image[6], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 7:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_text_image[7], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 8:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[1], TRUE);
			DrawGraph(0, 0, scenario_text_image[8], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 9:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[1], TRUE);
			DrawGraph(1160, 200, scenario_girl[1], TRUE);
			DrawGraph(0, 0, scenario_text_image[9], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 10:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[1], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_text_image[10], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 11:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[3], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_text_image[11], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 12:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[3], TRUE);
			DrawGraph(1160, 200, scenario_girl[3], TRUE);
			DrawGraph(0, 0, scenario_text_image[12], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 13:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[3], TRUE);
			DrawGraph(1160, 200, scenario_girl[1], TRUE);
			DrawGraph(0, 0, scenario_text_image[13], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 14:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[3], TRUE);
			DrawGraph(1160, 200, scenario_girl[0], TRUE);
			DrawGraph(0, 0, scenario_text_image[14], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 15:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[3], TRUE);
			DrawGraph(0, 0, scenario_text_image[15], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 16:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_text_image[16], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 17:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_text_image[17], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 18:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[3], TRUE);
			DrawGraph(0, 0, scenario_text_image[18], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 19:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[1], TRUE);
			DrawGraph(0, 0, scenario_text_image[19], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 20:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[3], TRUE);
			DrawGraph(0, 0, scenario_text_image[20], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 21:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[0], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_text_image[21], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		}
		break;

	case 2:	// ステージ

		//UI実装
		DrawGraph(0, 0, cgMap, TRUE);
		DrawGraph(1600, 0, operation, FALSE);			// 操作用画像（ボタンなど）
		DrawGraph(1600, 768, energy_ui_slime, TRUE);	// エネルギーUI（外側）
		DrawGraph(1680, 824, energy_ui_textbox, TRUE);	// エネルギーUI（内側）

		//エネルギーの表示
		left_up = plHP.current_hp / 10;
		right_up = plHP.current_hp % 10;
		if (plHP.current_hp < 10) {
			left_up = 0;
		}
		DrawGraph(1692, 840, energy_ui_number[left_up], TRUE);
		DrawGraph(1766, 840, energy_ui_number[right_up], TRUE);

		// プレイヤーのHP最大値の表示
		DrawGraph(1692, 904, energy_ui_number_Hpmax[1], TRUE);
		DrawGraph(1766, 904, energy_ui_number_Hpmax[0], TRUE);




		// 画像表示 ////////////////////////////////////////

		//// マップチップを for ループでひとつひとつ描画する
		int x, y;
		//マップチップを描画する
		for (y = 0; y < MAP_ROW; y++)
		{
			for (x = 0; x < MAP_COL; x++)
			{
				int drowX = x * CHIP_W;
				int drowY = y * CHIP_H;

				switch (map[y * MAP_W + x])
				{
				case 0:	// 床
					DrawGraph(drowX, drowY, cgChip[0], TRUE);
					break;
				case 1:		// 壁
					DrawGraph(drowX, drowY, cgChip[1], TRUE);
					break;
				case 8:		//透明
					break;	// 空白
				}

			}
		}

		for (y = 0; y < MAP_ROW; y++)
		{
			for (x = 0; x < MAP_COL; x++)
			{
				int drowX = x * CHIP_W;
				int drowY = y * CHIP_H;
				switch (item[y * MAP_W + x])
				{
				case 2:	// 食べ物(弱)
					DrawGraph(drowX, drowY, cgChip[2], TRUE);
					break;
				case 3:	// 食べ物(中)
					DrawGraph(drowX, drowY, cgChip[3], TRUE);
					break;
				case 4:	// 食べ物(強)
					DrawGraph(drowX, drowY, cgChip[4], TRUE);
					break;
				case 18:	// 食べ物(最強)
					DrawGraph(drowX, drowY, cgChip[6], TRUE);
					break;
				}
			}
		}

		// 箱もforループで描画する
		for (y = 0; y < MAP_H; y++) {
			for (x = 0; x < MAP_W; x++) {
				// box[] から、チップ番号を取り出す
				int chip_no = box[y * MAP_W + x];
				int drowX = x * CHIP_W;
				int drowY = y * CHIP_H;
				switch (chip_no)
				{
				case 5:	// 箱(弱)
					DrawGraph(drowX, drowY, cgBox[1], TRUE);
					break;
				case 6:	// 箱(中)
					DrawGraph(drowX, drowY, cgBox[0], TRUE);
					break;
				case 7:	// 箱(強)
					DrawGraph(drowX, drowY, cgBox[2], TRUE);
					break;
				}
			}
		}

		// ゴールの表示
		for (y = 0; y < MAP_ROW; y++)
		{
			for (x = 0; x < MAP_COL; x++)
			{
				int drowX = x * CHIP_W;
				int drowY = y * CHIP_H;
				switch (map[y * MAP_W + x])
				{
				case 10:	// ゴール1
					DrawGraph(drowX, drowY, cggoal[gStageNo][0], TRUE);
					break;
				case 11:	// ゴール2
					DrawGraph(drowX, drowY, cggoal[gStageNo][1], TRUE);
					break;
				case 12:	// ゴール3
					DrawGraph(drowX, drowY, cggoal[gStageNo][2], TRUE);
					break;
				case 13:	// ゴール4
					DrawGraph(drowX, drowY, cggoal[gStageNo][3], TRUE);
					break;
				case 14:	// 最終ゴール1
					DrawGraph(drowX, drowY, cggoal[gStageNo][0], TRUE);
					break;
				case 15:	// 最終ゴール2
					DrawGraph(drowX, drowY, cggoal[gStageNo][1], TRUE);
					break;
				case 16:	// 最終ゴール3
					DrawGraph(drowX, drowY, cggoal[gStageNo][2], TRUE);
					break;
				case 17:	// 最終ゴール4
					DrawGraph(drowX, drowY, cggoal[gStageNo][3], TRUE);
					break;

				}
			}
		}



		// プレイヤー
		// HPに応じて画像を切り替える

		if (plHP.current_hp >= 30) {
			playerImgIdx = 2;	// HPが30以上なら、画像は2番目
			DrawGraph(playerDrowX, playerDrowY, cgPlayer[playerImgIdx][chara.arrow * 3 + animTbl[(chara.animcnt / chara.animspeed) % 4]], TRUE);
		}
		else if (plHP.current_hp >= 20) {
			playerImgIdx = 1;	// HPが20?29なら、画像は1番目
			DrawGraph(playerDrowX, playerDrowY, cgPlayer[playerImgIdx][chara.arrow * 3 + animTbl[(chara.animcnt / chara.animspeed) % 4]], TRUE);
		}
		else if (plHP.current_hp >= 6) {
			playerImgIdx = 0;	// HPが6?19なら、画像は0番目

			DrawGraph(playerDrowX, playerDrowY, cgPlayer[playerImgIdx][chara.arrow * 3 + animTbl[(chara.animcnt / chara.animspeed) % 4]], TRUE);
		}
		else if (plHP.current_hp <= 5) {
			playerImgIdx = 3;	//HPが5以下なら,画像は3番目
			DrawGraph(playerDrowX, playerDrowY, cgPlayer[playerImgIdx][chara.arrow * 3 + animTbl[(chara.animcnt / chara.animspeed) % 4]], TRUE);
		}




		// プレイヤーのHPを表示
		/*DrawFormatString(10, 10, GetColor(255, 255, 255), "HP: %d/%d", plHP.current_hp, plHP.max_hp);*/


		//// 開発用
		//DrawFormatString(0, 350, GetColor(255, 0, 0), "stepsave_cnt = %d", stepsave_cnt);
		//for (int i = 0; i < stepsave_cnt; i++) {
		//	DrawFormatString(0, 370 + (16 * i), GetColor(255, 0, 0), "[%2d] player = (%d, %d)", i, stepsave_playerX[i], stepsave_playerY[i]);
		//}


		//////マップ配列の表示（開発者用）
		//for (int y = 0; y < MAP_H; y++)
		//{
		//	for (int x = 0; x < MAP_W; x++)
		//	{
		//		DrawFormatString(x * CHIP_W, y * CHIP_H, GetColor(rand() % 255, rand() % 255, rand() % 255), "%2d", stage[gStageNo][y * MAP_W + x]);
		//	}
		//}
		break;


	case 3: //全ステージクリア画面
		switch (clear_scenario) {
		case 0:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 0, scenario_slime[6], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 1:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 0, scenario_slime[7], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 2:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[4], TRUE);
			DrawGraph(1160, 200, scenario_girl[2], TRUE);
			DrawGraph(0, 0, scenario_clear_text_image[0], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 3:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[5], TRUE);
			DrawGraph(1160, 200, scenario_girl[3], TRUE);
			DrawGraph(0, 0, scenario_clear_text_image[1], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 4:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[4], TRUE);
			DrawGraph(1160, 200, scenario_girl[2], TRUE);
			DrawGraph(0, 0, scenario_clear_text_image[2], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 5:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[2], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_clear_text_image[3], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 6:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[2], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_clear_text_image[4], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 7:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[2], TRUE);
			DrawGraph(1160, 200, scenario_girl[3], TRUE);
			DrawGraph(0, 0, scenario_clear_text_image[5], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 8:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[4], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_clear_text_image[6], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 9:
			DrawGraph(0, 0, cgMap, TRUE);
			DrawGraph(120, 200, scenario_slime[5], TRUE);
			DrawGraph(1160, 200, scenario_girl[4], TRUE);
			DrawGraph(0, 0, scenario_clear_text_image[7], TRUE);
			DrawGraph(1600, 800, next_image, TRUE);
			break;
		case 10:
			DrawGraph(0, 0, game_clear_image, TRUE);
			//DrawGraph(1600, 800, next_image, TRUE);
			break;
		}
		break;

	case 4: //ステージクリア画面
		DrawGraph(0, 0, cgMap, TRUE);
		DrawGraph(420, 0, stage_clear_image, TRUE);	// ステージクリア画像を描画
		if (gTrg & PAD_INPUT_10) {
			if (gStageNo + 1 < STAGE_MAX) {
				gStageNo++;
				StageInit(gStageNo);
			}
			phase = 2;	// ステージフェーズへ戻る（挙動が正解かわからない）
		}
		break;


	case 5: //ゲームオーバー画面
		DrawGraph(420, 0, game_over_image, TRUE);	// ゲームオーバー画像を描画
		DrawGraph(1600, 800, next_image, TRUE);//リセットボタン
		if (gTrg & PAD_INPUT_10) {
			StageInit(gStageNo);	// ステージをリセット
			phase = 2;	// ステージフェーズへ戻る（挙動が正解かわからない）
		}
		break;


	default: // その他のフェーズはゲームを終了させる
		break;
	}

	ScreenFlip();
}


void GameMain()
{
	AppInit();	// 初期化

	// 1フレームループを組む ----------------------------------------------------------
	while (ProcessMessage() == 0 && !isGameEnd)		// プログラムが終了するまでループ
	{
		FrameInput();	// 入力
		FrameProcess();	// 計算
		FrameDraw();	// 描画
	}

	AppRelease();	// 解放
}