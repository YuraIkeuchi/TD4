#include "Helper.h"

//max min‚ğbool•ª‚Å•Ô‚·
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
//Clamp‚ğ•Ô‚·ŠÖ”
void Helper::IntClamp(int& Num,const int Min,const int Max) {
	Num = min(max(Num, Min), Max);
}

void Helper::FloatClamp(float& Num,const float Min,const float Max) {
	Num = min(max(Num, Min), Max);
}

//XMFLOAT3‚Æfloat‚ğ‰ÁZ‚·‚éŠÖ”
DirectX::XMFLOAT3 Helper::Float3AddFloat(const XMFLOAT3& Num, float Add) {
	XMFLOAT3 l_Result;
	 l_Result.x = Num.x + Add;
	 l_Result.y = Num.y + Add;
	 l_Result.z = Num.z + Add;
	
	 return l_Result;
}

//XMFLOAT3‚ÆXMFLOAT3‚ğ‰ÁZ‚·‚éŠÖ”
DirectX::XMFLOAT3 Helper::Float3AddFloat3(const XMFLOAT3& Num, const XMFLOAT3& Add) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x + Add.x;
	l_Result.y = Num.y + Add.y;
	l_Result.z = Num.z + Add.z;

	return l_Result;
}

//XMFLOAT3‚Æfloat‚ğŒ¸Z‚·‚éŠÖ”
DirectX::XMFLOAT3 Helper::Float3SubFloat(const XMFLOAT3& Num, float Sub) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x - Sub;
	l_Result.y = Num.y - Sub;
	l_Result.z = Num.z - Sub;

	return l_Result;
}

//XMFLOAT3‚ÆXMFLOAT3‚ğŒ¸Z‚·‚éŠÖ”
DirectX::XMFLOAT3 Helper::Float3SubFloat3(const XMFLOAT3& Num, const XMFLOAT3& Sub) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x - Sub.x;
	l_Result.y = Num.y - Sub.y;
	l_Result.z = Num.z - Sub.z;

	return l_Result;
}