#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
//���ʒ����p�̃N���X
class VolumManager {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static VolumManager* GetInstance();
	//�����Update�ȊO�g��Ȃ�
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
public:
	float GetSEVolum() { return   s_SEVolum; }
	float GetBGMVolum() { return   s_BGMVolum; }

	void SetSEVolum(float s_SEVolum) { this->s_SEVolum = s_SEVolum; }
	void SetBGMVolum(float s_BGMVolum) { this->s_BGMVolum = s_BGMVolum; }
private:
	//SE��BGM�̉���
	static float s_SEVolum;
	static float s_BGMVolum;
};