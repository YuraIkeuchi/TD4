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
	//////�L�[�{�[�h�̏�����
	ComPtr<IDirectInput8>dinput = nullptr;
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	////
	//�L�[�{�[�h�f�o�C�X�̍쐬
	////
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	//////�ɂイ��傭�f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);

	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	//�Q�[���p�b�h�̐���
	devGamePad = nullptr;
	result = dinput->CreateDevice(GUID_Joystick, &devGamePad, NULL);

	if (devGamePad != nullptr)
	{
		//���̓f�[�^�`���̃Z�b�g
		result = devGamePad->SetDataFormat(&c_dfDIJoystick2);

		// �����[�h���Βl���[�h�Ƃ��Đݒ�
		DIPROPDWORD diprop;
		ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;

		// �����[�h��ύX
		devGamePad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		// X���̒l�͈̔͐ݒ�
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwObj = DIJOFS_X;
		diprg.lMin = -1000;
		diprg.lMax = 1000;

		// X���̒l�͈̔͐ݒ�
		devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Y���̒l�͈̔͐ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

		//�r�����䃌�x���Z�b�g
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
		//�Q�[���p�b�h���̎擾�J�n
		result = devGamePad->Acquire();
		//�O�t���[���̔���
		oldGamePadState = gamePadState;
		//�S�{�^���̏����擾����
		result = devGamePad->GetDeviceState(sizeof(DIJOYSTATE2), &gamePadState);
		//���Z�b�g
		for (int i = 0; i < 32; i++)
		{
			is_push[i] = false;
		}
	}
}

bool Input::PushKey(BYTE keyNumber)
{
	//�w��L�[�������Ă����true��Ԃ�
	if (key[keyNumber]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) {

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

//�}�E�X�֌W
bool Input::PushMouseLeft()
{
	//0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[0]) {
		return true;
	}

	return false;
}

bool Input::PushMouseMiddle()
{
	//0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[2]) {
		return true;
	}

	return false;
}

bool Input::TriggerMouseLeft()
{	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0]) {
		return true;
	}

	return false;
}

bool Input::TriggerMouseMiddle()
{	// �O��0�ŁA����0�łȂ���΃g���K�[
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
//���X�e�B�b�N
bool Input::LeftTiltStick(const int& stick)
{
	
	//��
	if (gamePadState.lX < -unresponsive_range && stick == Left)
	{
		return true;
	}
	//�E
	else if (gamePadState.lX > unresponsive_range && stick == Right)
	{
		return true;
	}

	//���
	if (gamePadState.lY > unresponsive_range && stick == Down)
	{
		return true;
	}
	//�O
	else if (gamePadState.lY < -unresponsive_range && stick == Up)
	{
		return true;
	}
	posX = (float)gamePadState.lX;
	posY = (float)gamePadState.lY;

	DWORD length = 1000;//���_����ŏ��A�ő�܂ł̒���
	x_vec = ((float)gamePadState.lX - unresponsive_range) / (length - unresponsive_range);
	y_vec = ((float)gamePadState.lY - unresponsive_range) / (length - unresponsive_range);
	return false;
}
//���X�e�B�b�N(�g���K�[)
bool Input::LeftTriggerStick(const int& stick)
{
	//��
	if (gamePadState.lX < -unresponsive_range && !(oldGamePadState.lX < -unresponsive_range) && stick == Left)
	{
		return true;
	}
	//�E
	else if (gamePadState.lX > unresponsive_range && !(oldGamePadState.lX > unresponsive_range) && stick == Right)
	{
		return true;
	}
	//���
	if (gamePadState.lY > unresponsive_range && !(oldGamePadState.lY > unresponsive_range) && stick == Down)
	{
		return true;
	}
	//�O
	else if (gamePadState.lY < -unresponsive_range && !(oldGamePadState.lY < -unresponsive_range) && stick == Up)
	{
		return true;
	}

	return false;
}
//�E�X�e�B�b�N
bool Input::RightTiltStick(const int& stick)
{
	unresponsive_range = 600;
	//��
	if ((gamePadState.lRx - 32500) < -unresponsive_range && stick == Left)
	{
		return true;
	}
	//�E
	else if ((gamePadState.lRx - 32500) > unresponsive_range && stick == Right)
	{
		return true;
	}
	//���
	if ((gamePadState.lRy - 32500) > unresponsive_range && stick == Down)
	{
		return true;
	}
	//�O
	else if ((gamePadState.lRy - 32500) < -unresponsive_range && stick == Up)
	{
		return true;
	}
	posX = (float)gamePadState.lRx;
	posY = (float)gamePadState.lRy;

	return false;
}
//�E�X�e�B�b�N(�g���K�[)
bool Input::RightTriggerStick(const int& stick)
{
	unresponsive_range = 600;
	//��
	if ((gamePadState.lRx - 32500) < -unresponsive_range && !((oldGamePadState.lRx - 32500) < -unresponsive_range) && stick == Left)
	{
		return true;
	}
	//�E
	else if ((gamePadState.lRx - 32500) > unresponsive_range && !((oldGamePadState.lRx - 32500) > unresponsive_range) && stick == Right)
	{
		return true;
	}
	//���
	if ((gamePadState.lRy - 32500) > unresponsive_range && !((oldGamePadState.lRy - 32500) > unresponsive_range) && stick == Down)
	{
		return true;
	}
	//�O
	else if ((gamePadState.lRy - 32500) < -unresponsive_range && !((oldGamePadState.lRy - 32500) < -unresponsive_range) && stick == Up)
	{
		return true;
	}

	return false;
}
//�{�^��
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
//�{�^��(�g���K�[)
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
//�\���L�[
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
//�\���L�[(�g���K�[)
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
//�J��
void Input::Finalize() {
	devGamePad.Reset();
	devkeyboard.Reset();
	devMouse.Reset();
}