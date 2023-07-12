#pragma once
#include"IKESprite.h"
#include<memory>
#include<array>
#include<vector>
#include<DirectXMath.h>
#include"IKEObject3d.h"
#include "IKETexture.h"
#include "SceneManager.h"

using namespace DirectX;
using namespace std;
class SelectScene {
public:
	static SelectScene* GetIns();
	SelectScene() {};
	~SelectScene() {};

	enum Stage {
		FIRST,
		SECOND,
		THIRD,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		MAX,
	}_stages = MAX;
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
	void Draw_SpriteBack();
	void Draw_Obj(DirectXCommon* dxcomn);
	/**
	 * \brief 諸々リセット
	 */

	void CloseIconView(bool closeF);

	void ViewTips();

	/**
	 * \brief NONのとき初期化用
	 */
	void ResetParama();

	void TipsPosUpda(Stage stage);
private:
	//土台
	unique_ptr<IKEObject3d>Pedestal = nullptr;
	XMFLOAT3 PedestalRot = { 0,0,0 };

	//オブジェクト数（ステージ数）
	static constexpr int ObjNum = 7;

	array<unique_ptr<IKEObject3d>, ObjNum>StageObjs = { nullptr };
	array<unique_ptr<IKESprite>, ObjNum>StageObj = { nullptr };
	array<XMFLOAT3, ObjNum>StageObjPos = {};
	array<float, ObjNum>TipsPosY = {};
	array<bool, ObjNum>TipsAct = {};
	array<XMFLOAT3, ObjNum>StageObjRot = {};
	array<float, ObjNum>StageObjRotAngle = {};
	array<float, ObjNum>StageObjEaseT = {};
	array<float, ObjNum>IconColor = {};
	array<float, ObjNum> NowRotAngle = {};
	float IconRotAngle_EaseT = 0.0f;

	enum Select {
		NOINP,
		RB, LB
	}TrigerSelect = NOINP;
private:
	unique_ptr<IKEObject3d>BackSkyDome = nullptr;

	array<unique_ptr<IKESprite>, 2>ButtonNav_RBLB = {};
	array<unique_ptr<IKESprite>, 2>ButtonNav_Challenge_Cancel = {};
	array<unique_ptr<IKESprite>, ObjNum>StageName = {};
	array<unique_ptr<IKESprite>, ObjNum>BossIcon = {};

	array<XMFLOAT2, 2>ButtonNav_Challenge_CancelScl;
	array<float, 2>ButtonNav_Challenge_CancelColAlpha;
	array<XMFLOAT2, 2>ButtonNav_Pos = { };

private:
	bool ChangeF = false;
	bool JudgChal = false;
	bool sin = false;
	bool CloseF = false;

	int SelIndex = 0;

	float SkydomeRotY = 0;
	float closeScl = 6500.f;
	float closeRad = 1500.f;
	float SclingSpeed = 55.f, CorrSpeed = 0.48f;

	void RotPedestal();
	void ChangeEffect(std::string name, Stage stage, UINT iconnum);
public:
	float GetCloseIconRad() { return closeRad; }
	float GetCloseScl() { return closeScl; }
	XMFLOAT3 GetPedestalPos() { return Pedestal->GetPosition(); }
};

