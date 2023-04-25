#pragma once
#include "Camera.h"
/*--円周とか求める用--*/
#define PI 3.14f
#define PI_90 90.00f
#define PI_180 180.00f
#define PI_360 360.00f
/*-----------------*/

//補助クラス
class Helper
{
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static Helper* GetInstance();
	void SetCamera(Camera* camera) {
		m_Camera = camera;
	}
	//計算系
	bool CheckMaxFLOAT(float& Num, const float Max, const float Add);
	bool CheckMinFLOAT(float& Num, const float Min, const float Add);
	bool CheckMinINT(int& Num, const int Min, const int Add);
	bool CheckMaxINT(int& Num, const int Max, const int Add);
	//Clampを返す関数
	void IntClamp(int& Num, const int Min, const int Max);
	void FloatClamp(float& Num, const float Min, const float Max);
	//XMFLOAT3とfloatを加算する関数
	XMFLOAT3 Float3AddFloat(const XMFLOAT3& Num, const float Add);
	//XMFLOAT3とXMFLOAT3を加算する関数
	XMFLOAT3 Float3AddFloat3(const XMFLOAT3& Num, const XMFLOAT3& Add);
	//XMFLOAT3とfloatを減算する関数
	XMFLOAT3 Float3SubFloat(const XMFLOAT3& Num, const float Sub);
	//XMFLOAT3とXMFLOAT3を減算する関数
	XMFLOAT3 Float3SubFloat3(const XMFLOAT3& Num, const XMFLOAT3& Sub);

	//追従関数
	void FollowMove(XMFLOAT3& pos, const XMFLOAT3& pos2, const float vel);

	//円運動
	XMFLOAT3 CircleMove(const XMFLOAT3& basepos, const float scale, const float speed);
	//if文の判定の関数(値が範囲外にあるか)
	bool CheckNotValueRange(float& Num, const float Min, const float Max);
	//逆に範囲内にあるか
	bool CheckValueRange(float& Num, const float Min, const float Max);
public:
	//割合を返す
	float GetPercent(const float amount, const float num);

	//2D->3D変換に使う(スプライトの座標をプレイヤーのワールド座標に表示したりするのに使う)
	//座標系が明確でない場合->行列から計算->W除算
	XMVECTOR WDivision(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub);
	//座標系が明確な場合->そのまま計算->W除算
	XMVECTOR WDivision(const XMVECTOR& pos, const bool sub);
	//ワールド座標に変換
	XMVECTOR PosDivi(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub);
	//座標変換
	XMVECTOR WorldDivision(const XMVECTOR& pos, const bool sub);
	//3D座標を2Dに変える(レティクルなどの当たり判定などに使う
	XMFLOAT2 WorldtoScreen(const XMVECTOR& pos, const XMMATRIX& mat);
	//XMFLOATをXMVECTORに変換する
	XMVECTOR ChangeFLOAT(const XMFLOAT3& pos);
	//XMFLOATをXMVECTORに変換する
	XMFLOAT3 ChangeVECTOR(const XMVECTOR& pos);
	void ChangeViewPort(XMMATRIX& matviewport, const XMVECTOR& offset);
private:

	//カメラ
	Camera* m_Camera;

};

