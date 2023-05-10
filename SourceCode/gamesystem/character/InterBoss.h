#pragma once
#include "IKETexture.h"
#include "CollisionPrimitive.h"
#include "InterBullet.h"
#include "ObjCommon.h"
#include <array>       // ヘッダファイルインクルード
using namespace std;         //  名前空間指定

//ボスの基底クラス
class InterBoss :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	

public:

	//初期化
	virtual bool Initialize() = 0;
	//更新
	void Update();
	/// <summary>
	/// ポーズのとき
	/// </summary>
	virtual void Pause() = 0;
	//描画
	void Draw(DirectXCommon* dxCommon);

	virtual void Action() = 0;//ボス特有の処理

	virtual void ImGui_Origin() = 0;//ボスそれぞれのImGui

	void ImGuiDraw();//ImGuiの描画

	virtual void EffecttexDraw(DirectXCommon* dxCommon) = 0;
private:
protected:

	int ActionDamage;
	int ActionCool;

	float m_HP = {};
private:

	enum class ActionList
	{
		NON,
		NORMAL,
		MOB_A,
		MOB_B,
		FRONT
	}_action;

	struct DefaultParam
	{
		float Cool;
		int Damage;
		std::vector<float>ParSize;
		bool Shake;
	};
protected:
	//弾との当たり判定
	void CollideBul(vector<InterBullet*>bullet);

public:
	bool Recv;
};

