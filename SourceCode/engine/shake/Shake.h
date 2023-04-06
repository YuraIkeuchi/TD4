#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
//�V�F�C�N�p�̃N���X
class Shake {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	void ShakePos(float& pos, const int ShakeMax, const int ShakeMin, const int TargetTimer,const int Division);
public:
	//gettersetter
	bool GetShakeStart() { return  m_ShakeStart; }
	void SetShakeStart(bool ShakeStart) { this->m_ShakeStart = ShakeStart; }
private:
	int m_ShakeTimer = 0;
	bool m_ShakeStart = false;
};