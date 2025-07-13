#include "DxLib.h"
#include "winmain.h"
#include "gamemain.h"
#include "GamePad.h"


GamePad::GamePad() {
	_key = 0;
	_trg = 0;
	_rel = 0;
	// 複数ゲームパッド用
	for (int i = 0; i < MUTIPAD_MAX; i++) {
		_m[i]._key = 0;
		_m[i]._trg = 0;
		_m[i]._rel = 0;
	}
}

GamePad::~GamePad() {
}

void GamePad::Input() {
	// キー入力、トリガ入力、リリース入力を得る
	int keyold = _key;
	_key = GetJoypadInputState(DX_INPUT_KEY_PAD1);		// キー入力を取得
	_trg = (_key ^ keyold) & _key;
	_rel = (_key ^ keyold) & ~_key;

	// 複数ゲームパッド用
	int padTbl[] = {
		DX_INPUT_KEY,	// 0:キー入力
		DX_INPUT_PAD1,	// 1:パッド1
		DX_INPUT_PAD2,	// 2:パッド2
		DX_INPUT_PAD3,	// 3:パッド3
		DX_INPUT_PAD4,	// 4:パッド4
		DX_INPUT_PAD5,	// 5:パッド5
		DX_INPUT_PAD6,	// 6:パッド6
		DX_INPUT_PAD7,	// 7:パッド7
		DX_INPUT_PAD8,	// 8:パッド8
		DX_INPUT_PAD9,	// 9:パッド9
		DX_INPUT_PAD10,	// 10:パッド10
		DX_INPUT_PAD11,	// 11:パッド11
		DX_INPUT_PAD12,	// 12:パッド12
		DX_INPUT_PAD13,	// 13:パッド13
		DX_INPUT_PAD14,	// 14:パッド14
		DX_INPUT_PAD15,	// 15:パッド15
		DX_INPUT_PAD16,	// 16:パッド16
	};
	for (int i = 0; i < MUTIPAD_MAX; i++) {
		int keyold = _m[i]._key;
		_m[i]._key = GetJoypadInputState(padTbl[i]);
		_m[i]._trg = (_m[i]._key ^ keyold) & _m[i]._key;
		_m[i]._rel = (_m[i]._key ^ keyold) & ~_m[i]._key;
	}
}

