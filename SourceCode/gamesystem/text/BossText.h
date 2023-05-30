#pragma once
#include "TextManager.h"

//“G‚ÌŠÇ—ŒnƒNƒ‰ƒX
class BossText {
private:
	// DirectX::‚ğÈ—ª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	BossText();
	//‰Šú‰»
	void Initialize(DirectXCommon* dxCommon);
	void SpriteDraw(DirectXCommon* dxCommon);
private:
	unique_ptr<TextManager> text_;
};