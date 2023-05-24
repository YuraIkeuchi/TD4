#pragma once
#include "IKETexture.h"
#include "CollisionPrimitive.h"
#include "InterBullet.h"
#include "ObjCommon.h"
#include "BreakEffect.h"
#include <array>       // ヘッダファイルインクルード

#include "InterEnemy.h"
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
	bool GetIsAlive() { return isAlive; }
	//gettersetter
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
	virtual void Draw(DirectXCommon* dxCommon)override;

	virtual void Action() = 0;//ボス特有の処理

	virtual void ImGui_Origin() = 0;//ボスそれぞれのImGui

	void ImGuiDraw();//ImGuiの描画

	virtual void EffecttexDraw(DirectXCommon* dxCommon) = 0;

	void BirthEffect();

public:
	void SummonEnemyInit(InterEnemy* enemy);

	void SummonEnemyUpda(std::vector<InterEnemy*> enemy);

	void SummonEnemyDraw(std::vector<InterEnemy*> enemy, DirectXCommon* dxcomn);

	void EndSummon(std::vector<InterEnemy*> enemy);
	void isRespawn(std::vector<InterEnemy*> enemy);
protected:
	bool SummonF;
	bool SummobnStop;
public://gettersetter
	void SetHP(float hp) { m_HP = hp; };
	float GetHP() { return m_HP; }
	float HpPercent();

	void SetCheck(bool Check) { m_Check = Check; };
	bool GetCheck() { return m_Check; }

private:
	std::string SceneName;
	vector<InterEffect*> effects;
protected:
	//ダメージ食らったとの色変換
	float ColChangeEaseT;
	int ActionTimer;

	int m_CheckTimer = {};

	int ActionDamage;
	int ActionCool;

	bool isAlive;
	float m_HP = {};
	float m_MaxHp = {};

	bool m_Check = false;
	XMFLOAT3 m_OBBScale = {};
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
	bool EndSummonRepos;
	bool ResF;
public:
	bool Recv;
	int NextActionInteval;
};

