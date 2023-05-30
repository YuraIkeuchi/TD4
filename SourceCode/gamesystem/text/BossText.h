#pragma once
#include "TextManager.h"

//�G�̊Ǘ��n�N���X
class BossText {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	BossText();
	//������
	void Initialize(DirectXCommon* dxCommon);
	void SpriteDraw(DirectXCommon* dxCommon);
private:
	unique_ptr<TextManager> text_;
};