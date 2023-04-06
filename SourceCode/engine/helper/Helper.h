#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <DirectXMath.h>

//•â•ƒNƒ‰ƒX
class Helper
{
private:
	// DirectX::‚ğÈ—ª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//ŒvZŒn
	bool CheckMax(float& Num, const float Max, const float Add);
	bool CheckMin(float& Num, const float Min, const float Add);
	//Clamp‚ğ•Ô‚·ŠÖ”
	void IntClamp(int& Num, const int Min, const int Max);
	void FloatClamp(float& Num, const float Min, const float Max);
	//XMFLOAT3‚Æfloat‚ğ‰ÁZ‚·‚éŠÖ”
	XMFLOAT3 Float3AddFloat(const XMFLOAT3& Num, const float Add);
	//XMFLOAT3‚ÆXMFLOAT3‚ğ‰ÁZ‚·‚éŠÖ”
	XMFLOAT3 Float3AddFloat3(const XMFLOAT3& Num, const XMFLOAT3& Add);
	//XMFLOAT3‚Æfloat‚ğŒ¸Z‚·‚éŠÖ”
	XMFLOAT3 Float3SubFloat(const XMFLOAT3& Num, const float Sub);
	//XMFLOAT3‚ÆXMFLOAT3‚ğŒ¸Z‚·‚éŠÖ”
	XMFLOAT3 Float3SubFloat3(const XMFLOAT3& Num, const XMFLOAT3& Sub);
};

