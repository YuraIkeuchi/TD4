#pragma once
#include <wrl.h>
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800//DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"
class Input
{
public:
	//�Q�[���p�b�h�i�{�^���j
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

	//�X�e�B�b�N
	enum StickKind
	{
		Up, Down, Right, Left
	};

	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static Input* GetInstance();
	//������
	void Initialize(WinApp* winApp);
	//�X�V
	//�L�[�{�[�h
	void Update();
	void Finalize();
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);
	//�}�E�X
	bool PushMouseLeft();
	bool PushMouseMiddle();
	bool TriggerMouseLeft();
	bool TriggerMouseMiddle();
	MouseMove GetMouseMove();
	//�Q�[���p�b�h
	bool LeftTiltStick(const int& stick);
	bool LeftTriggerStick(const int& stick);
	bool RightTiltStick(const int& stick);
	bool RightTriggerStick(const int& stick);
	bool PushButton(const int& Button);
	bool TriggerButton(const int& Button);
	bool PushCrossKey(const int& CrossKey);
	bool TriggerCrossKey(const int& CrossKey);
public: // �����o�֐�
	const float& GetPosX() { return posX; }
	const float& GetPosY() { return posY; }
	const float& GetVecX() { return x_vec; }
	const float& GetVecY() { return y_vec; }
private://�����o�ϐ�
	//�L�[�{�[�h�̃f�p�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
	//�Q�[���p�b�h�f�o�C�X
	ComPtr<IDirectInputDevice8> devGamePad;
	DIJOYSTATE2 gamePadState = {};
	DIJOYSTATE2 oldGamePadState = {};
	DIJOYSTATE2 gamePadState2 = {};
	DIJOYSTATE2 oldGamePadState2 = {};
	bool is_push[32] = {};
	//�X�e�B�b�N�̖������͈�
	LONG unresponsive_range = 640;
	LONG unresponsive_range2 = 200;
	float posX = 0;
	float posY = 0;
	float x_vec = 0.0f;
	float y_vec = 0.0f;
private:
	WinApp* winApp = nullptr;
};