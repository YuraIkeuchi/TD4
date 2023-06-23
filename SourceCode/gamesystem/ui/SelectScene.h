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
class SelectScene
{
public:
	static SelectScene* GetIns();
	SelectScene(){};
	~SelectScene(){};

	enum Stage
	{
		
		FIRST,
		SECOND,
		THIRD,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		NON,
	}_stages = NON;
public:
	/**
	 * \brief ������
	 */
	void Init();

	/**
	 * \brief �X�V
	 */
	void Upda();

	/**
	 * \brief �`��
	 */
	void Draw_Sprite();
	void Draw_SpriteBack();
	void Draw_Obj(DirectXCommon* dxcomn);
	/**
	 * \brief ���X���Z�b�g
	 */
	void ResetParam();
	//
	void SceneChange( SceneChanger* schange);

	void CloseIconView(bool closeF);

	void ViewTips();

	/**
	 * \brief NON�̂Ƃ��������p
	 */
	void ResetParama();

	void TipsPosUpda(Stage stage);
private:
	//�y��
	unique_ptr<IKEObject3d>Pedestal=nullptr;
	XMFLOAT3 PedestalRot={0,0,0};

	//�I�u�W�F�N�g���i�X�e�[�W���j
	static constexpr int ObjNum = 7;

	array<unique_ptr<IKEObject3d>, ObjNum>StageObjs = { nullptr };
	array<unique_ptr<IKESprite>, ObjNum>StageObj={nullptr};
	array<XMFLOAT3, ObjNum>StageObjPos;
	array<float, ObjNum>TipsPosY;
	array<bool, ObjNum>TipsAct;
	array<XMFLOAT3, ObjNum>StageObjRot;
	array<float, ObjNum>StageObjRotAngle;
	array<float, ObjNum>StageObjEaseT;
	array<float, ObjNum>IconColor;
	float IconRotAngle_EaseT;
	array<float,ObjNum> NowRotAngle;

	enum Select
	{
		NOINP,
		RB,LB
	}TrigerSelect=NOINP;

	int SelIndex=0;

	unique_ptr<IKEObject3d>BackSkyDome;
	float SkydomeRotY = 0;
private:
	array<unique_ptr<IKESprite>, 2>ButtonNav_RBLB;
	array<unique_ptr<IKESprite>, 2>ButtonNav_Challenge_Cancel;
	array<XMFLOAT2, 2>ButtonNav_Challenge_CancelScl;
	array<float, 2>ButtonNav_Challenge_CancelColAlpha;
	bool JudgChal,JudgCancel;

	array<unique_ptr<IKESprite>, ObjNum>StageName;
	array<unique_ptr<IKESprite>, ObjNum>BossIcon;
private:
	//��]�A���O��
	float AngleRot;
	//
	int index;
	//����p
	bool YESorNOflag;

	bool ChangeF = false;

	float closeScl = 6500.f;
	float closeRad = 1500.f;
	bool k;
	bool sin = false;
	float SclingSpeed = 55.f, CorrSpeed = 0.48f;
	bool CloseF;
	void SetStage(bool judg,string sceneName);

	void RotPedestal();

	void ChangeEffect(std::string name,Stage stage,UINT iconnum);
public:
	float GetCloseIconRad() { return closeRad; }
	float GetCloseScl() { return closeScl; }
	XMFLOAT3 GetPedestalPos() { return Pedestal->GetPosition(); }
	int GetNowIndex() { return index; }
};

