#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
//音量調整用のクラス
class VolumManager {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static VolumManager* GetInstance();
	//今回はUpdate以外使わない
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
	//SEとBGMの音量
	static float s_SEVolum;
	static float s_BGMVolum;
};