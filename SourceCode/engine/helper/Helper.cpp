#include "Helper.h"

//max minをbool分で返す
bool Helper::CheckMax(float& Num, const float Max,const float Add) {
	Num += Add;

	Num = max(Num, Max);


	if (Num <= Max) {
		return true;
	}
	else {
		return false;
	}

	return false;
}

bool Helper::CheckMin(float& Num, const float Min, const float Add) {
	Num += Add;
	Num = min(Num, Min);

	if (Num >= Min) {
		return true;
	}
	else {
		return false;
	}

	return false;
}
//Clampを返す関数
void Helper::IntClamp(int& Num,const int Min,const int Max) {
	Num = min(max(Num, Min), Max);
}

void Helper::FloatClamp(float& Num,const float Min,const float Max) {
	Num = min(max(Num, Min), Max);
}

//XMFLOAT3とfloatを加算する関数
DirectX::XMFLOAT3 Helper::Float3AddFloat(const XMFLOAT3& Num, float Add) {
	XMFLOAT3 l_Result;
	 l_Result.x = Num.x + Add;
	 l_Result.y = Num.y + Add;
	 l_Result.z = Num.z + Add;
	
	 return l_Result;
}

//XMFLOAT3とXMFLOAT3を加算する関数
DirectX::XMFLOAT3 Helper::Float3AddFloat3(const XMFLOAT3& Num, const XMFLOAT3& Add) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x + Add.x;
	l_Result.y = Num.y + Add.y;
	l_Result.z = Num.z + Add.z;

	return l_Result;
}

//XMFLOAT3とfloatを減算する関数
DirectX::XMFLOAT3 Helper::Float3SubFloat(const XMFLOAT3& Num, float Sub) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x - Sub;
	l_Result.y = Num.y - Sub;
	l_Result.z = Num.z - Sub;

	return l_Result;
}

//XMFLOAT3とXMFLOAT3を減算する関数
DirectX::XMFLOAT3 Helper::Float3SubFloat3(const XMFLOAT3& Num, const XMFLOAT3& Sub) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x - Sub.x;
	l_Result.y = Num.y - Sub.y;
	l_Result.z = Num.z - Sub.z;

	return l_Result;
}

//割合を返す
float Helper::GetPercent(const float amount, const float num)
{
	return num / amount * 100.00f;
}

//2D->3D変換に使う(スプライトの座標をプレイヤーのワールド座標に表示したりするのに使う)
Helper::XMVECTOR Helper::WDivision(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub)
{
	float x, y, z, w;

	x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat.r[2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
	y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat.r[2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
	z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat.r[2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

	w = pos.m128_f32[3];
	if (sub){
		w = z;
	}

	x = x / w;
	y = y / w;
	z = z / w;

	return XMVECTOR{ x, y, z, w };
}

Helper::XMVECTOR Helper::WDivision(const XMVECTOR& pos, const bool sub)
{
	float x, y, z, w;
	x = pos.m128_f32[0];
	y = pos.m128_f32[1];
	z = pos.m128_f32[2];
	w = pos.m128_f32[3];

	if (sub)
	{
		w = z;
	}

	x = x / w;
	y = y / w;
	z = z / w;

	return XMVECTOR{ x, y, z, w };
}


Helper::XMVECTOR Helper::PosDivi(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub)
{
	float x, y, z, w;

	x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat.r[2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
	y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat.r[2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
	z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat.r[2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

	w = 1.0f;

	if (sub)
	{
		w = z;
	}

	return XMVECTOR{ x, y, z, w };
}
