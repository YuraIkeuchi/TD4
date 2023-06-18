#pragma once
#include"IKESprite.h"
#include<memory>
#include<array>
#include<vector>
#include<DirectXMath.h>
#include"IKEObject3d.h"
#include "IKETexture.h"

using namespace DirectX;
using namespace std;
class SelectScene
{
public:
	static SelectScene* GetIns();
	SelectScene(){};
	~SelectScene(){};
public:
	/**
	 * \brief 初期化
	 */
	void Init();

	/**
	 * \brief 更新
	 */
	void Upda();

	/**
	 * \brief 描画
	 */
	void Draw_Sprite();
	void Draw_Obj(DirectXCommon* dxcomn);
	/**
	 * \brief 諸々リセット
	 */
	void ResetParam();
private:
	//土台
	unique_ptr<IKEObject3d>Pedestal=nullptr;
	XMFLOAT3 PedestalRot={0,0,0};

	//オブジェクト数（ステージ数）
	static constexpr int ObjNum = 7;

	array<unique_ptr<IKETexture>, ObjNum>StageObj={nullptr};
	array<XMFLOAT3, ObjNum>StageObjPos;
	array<float, ObjNum>StageObjRotAngle;
	array<float, ObjNum>StageObjEaseT;
	array<float, ObjNum>IconColor;
	float IconRotAngle_EaseT;
	array<float,ObjNum> NowRotAngle;

	enum Select
	{
		NON,
		RB,LB
	}TrigerSelect=NON;
	
	enum Stage
	{
		FIRST,
		SECOND,
		THIRD,
		FOUR,
		FIVE,
		SIX,
		SEVEN
	}_stages=FIRST;

	int SelIndex=0;
	unique_ptr<IKEObject3d>BackSkyDome;
private:
	array<unique_ptr<IKESprite>, 2>ButtonNav_RBLB;
	array<unique_ptr<IKESprite>, ObjNum>StageName;
	array<unique_ptr<IKESprite>, ObjNum>BossIcon;
private:
	//回転アングル
	float AngleRot;
	//
	int index;
	//決定用
	bool YESorNOflag;

	void SetStage(bool judg,string sceneName);

	void RotPedestal();

public:
	XMFLOAT3 GetPedestalPos() { return Pedestal->GetPosition(); }
	int GetNowIndex() { return index; }
};

