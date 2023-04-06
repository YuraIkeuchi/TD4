#pragma once
#include <wrl.h>
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#include "WinApp.h"
class Input
{
public:
	//ゲームパッド（ボタン）
	enum ButtonKind
	{
		Button_A,
		Button_B,
		Button_X,
		Button_Y,
		Button_LB,
		Button_RB,
		Select,
		Start,
		Button_LeftStick,
		Button_RightStick,
		Cross_Up,
		Cross_Down,
		Cross_Right,
		Cross_Left,
		ButtonMax
	};

	//スティック
	enum StickKind
	{
		Up, Down, Right, Left
	};

	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static Input* GetInstance();
	//初期化
	void Initialize(WinApp* winApp);
	//更新
	//キーボード
	void Update();
	void Finalize();
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);
	//マウス
	bool PushMouseLeft();
	bool PushMouseMiddle();
	bool TriggerMouseLeft();
	bool TriggerMouseMiddle();
	MouseMove GetMouseMove();
	//ゲームパッド
	bool LeftTiltStick(const int& stick);
	bool LeftTriggerStick(const int& stick);
	bool RightTiltStick(const int& stick);
	bool RightTriggerStick(const int& stick);
	bool PushButton(const int& Button);
	bool TriggerButton(const int& Button);
	bool PushCrossKey(const int& CrossKey);
	bool TriggerCrossKey(const int& CrossKey);
public: // メンバ関数
	const float& GetPosX() { return posX; }
	const float& GetPosY() { return posY; }
	const float& GetVecX() { return x_vec; }
	const float& GetVecY() { return y_vec; }
private://メンバ変数
	//キーボードのデパイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
	//ゲームパッドデバイス
	ComPtr<IDirectInputDevice8> devGamePad;
	DIJOYSTATE2 gamePadState = {};
	DIJOYSTATE2 oldGamePadState = {};
	DIJOYSTATE2 gamePadState2 = {};
	DIJOYSTATE2 oldGamePadState2 = {};
	bool is_push[32] = {};
	//スティックの無反応範囲
	LONG unresponsive_range = 640;
	LONG unresponsive_range2 = 200;
	float posX = 0;
	float posY = 0;
	float x_vec = 0.0f;
	float y_vec = 0.0f;
private:
	WinApp* winApp = nullptr;
};