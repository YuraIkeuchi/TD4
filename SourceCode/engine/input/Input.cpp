#include "Input.h"
#pragma comment(lib,"dinput8.lib") 
#pragma comment(lib,"dxguid.lib") 


Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

void Input::Initialize(WinApp* winApp)
{
	this->winApp = winApp;
	HRESULT result;
	//////キーボードの初期化
	ComPtr<IDirectInput8>dinput = nullptr;
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	////
	//キーボードデバイスの作成
	////
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	//////にゅうりょくデータ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);

	//排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	//ゲームパッドの生成
	devGamePad = nullptr;
	result = dinput->CreateDevice(GUID_Joystick, &devGamePad, NULL);

	if (devGamePad != nullptr)
	{
		//入力データ形式のセット
		result = devGamePad->SetDataFormat(&c_dfDIJoystick2);

		// 軸モードを絶対値モードとして設定
		DIPROPDWORD diprop;
		ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;

		// 軸モードを変更
		devGamePad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		// X軸の値の範囲設定
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwObj = DIJOFS_X;
		diprg.lMin = -1000;
		diprg.lMax = 1000;

		// X軸の値の範囲設定
		devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Y軸の値の範囲設定
		diprg.diph.dwObj = DIJOFS_Y;
		devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

		//排他制御レベルセット
		result = devGamePad->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}

}

void Input::Update() 
{
	HRESULT result;
	memcpy(keyPre, key, sizeof(key));
	result = devkeyboard->Acquire();
	//BYTE key[256] = {};
	result = devkeyboard->GetDeviceState(sizeof(key), key);
	if (devGamePad != nullptr)
	{
		//ゲームパッド情報の取得開始
		result = devGamePad->Acquire();
		//前フレームの判定
		oldGamePadState = gamePadState;
		//全ボタンの情報を取得する
		result = devGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &gamePadState);
		//リセット
		for (int i = 0; i < 32; i++)
		{
			is_push[i] = false;
		}
	}
}

bool Input::PushKey(BYTE keyNumber)
{
	//指定キーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) {

	// 前回が0で、今回が0でなければトリガー
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// トリガーでない
	return false;
}

//マウス関係
bool Input::PushMouseLeft()
{
	//0でなければ押している
	if (mouseState.rgbButtons[0]) {
		return true;
	}

	return false;
}

bool Input::PushMouseMiddle()
{
	//0でなければ押している
	if (mouseState.rgbButtons[2]) {
		return true;
	}

	return false;
}

bool Input::TriggerMouseLeft()
{	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0]) {
		return true;
	}

	return false;
}

bool Input::TriggerMouseMiddle()
{	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2]) {
		return true;
	}

	return false;
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;

	return tmp;
}
//左スティック
bool Input::LeftTiltStick(const int& stick)
{
	
	//左
	if (gamePadState.lX < -unresponsive_range && stick == Left)
	{
		return true;
	}
	//右
	else if (gamePadState.lX > unresponsive_range && stick == Right)
	{
		return true;
	}

	//後ろ
	if (gamePadState.lY > unresponsive_range && stick == Down)
	{
		return true;
	}
	//前
	else if (gamePadState.lY < -unresponsive_range && stick == Up)
	{
		return true;
	}
	posX = (float)gamePadState.lX;
	posY = (float)gamePadState.lY;

	DWORD length = 1000;//原点から最小、最大までの長さ
	x_vec = ((float)gamePadState.lX - unresponsive_range) / (length - unresponsive_range);
	y_vec = ((float)gamePadState.lY - unresponsive_range) / (length - unresponsive_range);
	return false;
}
//左スティック(トリガー)
bool Input::LeftTriggerStick(const int& stick)
{
	//左
	if (gamePadState.lX < -unresponsive_range && !(oldGamePadState.lX < -unresponsive_range) && stick == Left)
	{
		return true;
	}
	//右
	else if (gamePadState.lX > unresponsive_range && !(oldGamePadState.lX > unresponsive_range) && stick == Right)
	{
		return true;
	}
	//後ろ
	if (gamePadState.lY > unresponsive_range && !(oldGamePadState.lY > unresponsive_range) && stick == Down)
	{
		return true;
	}
	//前
	else if (gamePadState.lY < -unresponsive_range && !(oldGamePadState.lY < -unresponsive_range) && stick == Up)
	{
		return true;
	}

	return false;
}
//右スティック
bool Input::RightTiltStick(const int& stick)
{
	unresponsive_range = 600;
	//左
	if ((gamePadState.lRx - 32500) < -unresponsive_range && stick == Left)
	{
		return true;
	}
	//右
	else if ((gamePadState.lRx - 32500) > unresponsive_range && stick == Right)
	{
		return true;
	}
	//後ろ
	if ((gamePadState.lRy - 32500) > unresponsive_range && stick == Down)
	{
		return true;
	}
	//前
	else if ((gamePadState.lRy - 32500) < -unresponsive_range && stick == Up)
	{
		return true;
	}
	posX = (float)gamePadState.lRx;
	posY = (float)gamePadState.lRy;

	return false;
}
//右スティック(トリガー)
bool Input::RightTriggerStick(const int& stick)
{
	unresponsive_range = 600;
	//左
	if ((gamePadState.lRx - 32500) < -unresponsive_range && !((oldGamePadState.lRx - 32500) < -unresponsive_range) && stick == Left)
	{
		return true;
	}
	//右
	else if ((gamePadState.lRx - 32500) > unresponsive_range && !((oldGamePadState.lRx - 32500) > unresponsive_range) && stick == Right)
	{
		return true;
	}
	//後ろ
	if ((gamePadState.lRy - 32500) > unresponsive_range && !((oldGamePadState.lRy - 32500) > unresponsive_range) && stick == Down)
	{
		return true;
	}
	//前
	else if ((gamePadState.lRy - 32500) < -unresponsive_range && !((oldGamePadState.lRy - 32500) < -unresponsive_range) && stick == Up)
	{
		return true;
	}

	return false;
}
//ボタン
bool Input::PushButton(const int& Button)
{
	for (int i = 0; i < 32; i++)
	{
		if (!(gamePadState.rgbButtons[i] & 0x80))
		{
			continue;
		}

		switch (i)
		{
		case 0:
			is_push[ButtonKind::Button_A] = true;
			break;
		case 1:
			is_push[ButtonKind::Button_B] = true;
			break;
		case 2:
			is_push[ButtonKind::Button_X] = true;
			break;
		case 3:
			is_push[ButtonKind::Button_Y] = true;
			break;
		case 4:
			is_push[ButtonKind::Button_LB] = true;
			break;
		case 5:
			is_push[ButtonKind::Button_RB] = true;
			break;
		case 6:
			is_push[ButtonKind::Select] = true;
			break;
		case 7:
			is_push[ButtonKind::Start] = true;
			break;
		case 8:
			is_push[ButtonKind::Button_LeftStick] = true;
			break;
		case 9:
			is_push[ButtonKind::Button_RightStick] = true;
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < Cross_Up; i++)
	{
		if (is_push[i] == true)
		{
			if (is_push[i] == is_push[Button])
			{
				return true;
			}
		}
	}

	return false;
}
//ボタン(トリガー)
bool Input::TriggerButton(const int& Button)
{
	for (int i = 0; i < 32; i++)
	{
		if (!(gamePadState.rgbButtons[i] & 0x80))
		{
			continue;
		}
		if (oldGamePadState.rgbButtons[i] & 0x80)
		{
			continue;
		}

		switch (i)
		{
		case 0:
			is_push[ButtonKind::Button_A] = true;
			break;
		case 1:
			is_push[ButtonKind::Button_B] = true;
			break;
		case 2:
			is_push[ButtonKind::Button_X] = true;
			break;
		case 3:
			is_push[ButtonKind::Button_Y] = true;
			break;
			is_push[ButtonKind::Button_Y] = true;
			break;
		case 4:
			is_push[ButtonKind::Button_LB] = true;
			break;
		case 5:
			is_push[ButtonKind::Button_RB] = true;
			break;
		case 6:
			is_push[ButtonKind::Select] = true;
			break;
		case 7:
			is_push[ButtonKind::Start] = true;
			break;
		case 8:
			is_push[ButtonKind::Button_LeftStick] = true;
			break;
		case 9:
			is_push[ButtonKind::Button_RightStick] = true;
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < Cross_Up; i++)
	{
		if (is_push[i] == true)
		{
			if (is_push[i] == is_push[Button])
			{
				return true;
			}
		}
	}

	return false;
}
//十字キー
bool Input::PushCrossKey(const int& CrossKey)
{
	if (gamePadState.rgdwPOV[0] != 0xFFFFFFFF)
	{
		switch (gamePadState.rgdwPOV[0])
		{
		case 0:
			is_push[ButtonKind::Cross_Up] = true;
			break;
		case 4500:
			is_push[ButtonKind::Cross_Up] = true;
			is_push[ButtonKind::Cross_Right] = true;
			break;
		case 9000:
			is_push[ButtonKind::Cross_Right] = true;
			break;
		case 13500:
			is_push[ButtonKind::Cross_Right] = true;
			is_push[ButtonKind::Cross_Down] = true;
			break;
		case 18000:
			is_push[ButtonKind::Cross_Down] = true;
			break;
		case 22500:
			is_push[ButtonKind::Cross_Down] = true;
			is_push[ButtonKind::Cross_Left] = true;
			break;
		case 27000:
			is_push[ButtonKind::Cross_Left] = true;
			break;
		case 31500:
			is_push[ButtonKind::Cross_Left] = true;
			is_push[ButtonKind::Cross_Up] = true;
			break;
		}

		for (int i = Cross_Up; i < ButtonMax; i++)
		{
			if (is_push[i] == true)
			{
				if (is_push[i] == is_push[CrossKey])
				{
					return true;
				}
			}
		}
	}

	return false;
}
//十字キー(トリガー)
bool Input::TriggerCrossKey(const int& CrossKey)
{
	if (gamePadState.rgdwPOV[0] != 0xFFFFFFFF && oldGamePadState.rgdwPOV[0] == 0xFFFFFFFF)
	{
		switch (gamePadState.rgdwPOV[0])
		{
		case 0:
			is_push[ButtonKind::Cross_Up] = true;
			break;
		case 4500:
			is_push[ButtonKind::Cross_Up] = true;
			is_push[ButtonKind::Cross_Right] = true;
			break;
		case 9000:
			is_push[ButtonKind::Cross_Right] = true;
			break;
		case 13500:
			is_push[ButtonKind::Cross_Right] = true;
			is_push[ButtonKind::Cross_Down] = true;
			break;
		case 18000:
			is_push[ButtonKind::Cross_Down] = true;
			break;
		case 22500:
			is_push[ButtonKind::Cross_Down] = true;
			is_push[ButtonKind::Cross_Left] = true;
			break;
		case 27000:
			is_push[ButtonKind::Cross_Left] = true;
			break;
		case 31500:
			is_push[ButtonKind::Cross_Left] = true;
			is_push[ButtonKind::Cross_Up] = true;
			break;
		}

		for (int i = Cross_Up; i < ButtonMax; i++)
		{
			if (is_push[i] == true)
			{
				if (is_push[i] == is_push[CrossKey])
				{
					return true;
				}
			}
		}
	}

	return false;
}
//開放
void Input::Finalize() {
	devGamePad.Reset();
	devkeyboard.Reset();
	devMouse.Reset();
}