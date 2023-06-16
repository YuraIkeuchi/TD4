#pragma once
#include "ObjCommon.h"
enum CDState {
	CD_BIRTH,
	CD_STAY,
	CD_THROUGH,
	CD_CATCH,
	CD_THROW,
	CD_DEATH,
	CD_RESPORN,
};
using namespace std;         //  名前空間指定
//音符クラス
class InterCD :
public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//初期化
	virtual bool Initialize() = 0;
	//更新
	void Update();
	//描画
	void Draw(DirectXCommon* dxCommon);

	//描画(固有の)
	virtual void Origin_Draw(DirectXCommon* dxCommon) = 0;

	void ImGuiDraw();//ImGuiの描画

protected:

	virtual void Action() = 0;//ボス特有の処理

	virtual void ImGui_Origin() = 0;//ボスそれぞれのImGui

protected:
	//メンバ関数
	virtual void BirthCD() {};
	virtual void StayCD() {};
	virtual void ThroughCD() {};
	virtual void CatchCD() {};
	virtual void ThrowCD() {};
	virtual void DeathCD() {};
	virtual void ResPornCD() {};
public:
	//gettersetter
	const int& GetCDState() { return m_CDState; }

	void SetCDState(const int CDState) { m_CDState = CDState; }

	void SetCatchPos(const XMFLOAT3 CatchPos) { m_CatchPos = CatchPos; }
protected:
	static void (InterCD::* stateTable[])();
private:
protected:
	//CDの状態
	int m_CDState = CD_BIRTH;

	//上昇度
	float m_AddPower = 0.0f;
	//重力加速度
	float m_Gravity = 0.02f;
	//キャッチした後のポジション
	XMFLOAT3 m_CatchPos = {};
	//投げる間の時間
	int m_ThrowTimer = {};
	double m_SpeedX = 0.0f;
	double m_SpeedZ = 0.0f;

	//リスポーン時間
	int m_ResPornTimer = {};
};