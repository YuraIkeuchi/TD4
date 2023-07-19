#pragma once
#include"IKESprite.h"
#include<memory>
#include<array>
#include<vector>
#include<DirectXMath.h>
#include"IKEObject3d.h"
#include "IKETexture.h"
#include "SceneManager.h"
enum SelectState {
	SELECT_FIRST,//�ŏ��̋����̂�
	SELECT_SECOND,//�炷�ڂ��ȊO�J���ς�
	SELECT_LAST,//���X�{�X�J��
};

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
		TITLE,
		MAX,
	}_stages = MAX;
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

	void CloseIconView(bool closeF);

	void ViewTips();

	/**
	 * \brief NON�̂Ƃ��������p
	 */
	void ResetParama();

	void TipsPosUpda(Stage stage);

	void BirthParticle();

	void ImGuiDraw();

private:
	//�󋵂ɉ����ĊǗ�����
	void StateManager();
private:
	//�y��
	unique_ptr<IKEObject3d>Pedestal = nullptr;
	XMFLOAT3 PedestalRot = { 0,0,0 };

	//�I�u�W�F�N�g���i�X�e�[�W���j
	static constexpr int ObjNum = 8;

	array<unique_ptr<IKEObject3d>, ObjNum>StageObjs = { nullptr };
	array<unique_ptr<IKESprite>, ObjNum>StageObj = { nullptr };
	array<XMFLOAT3, ObjNum>StageObjPos = {};
	array<XMFLOAT3, ObjNum>m_Scale = {};
	array<XMFLOAT3, ObjNum>AfterScale = {};
	array<float, ObjNum>TipsPosY = {};
	array<bool, ObjNum>TipsAct = {};
	array<bool, ObjNum>m_Birth = {};
	array<bool, ObjNum>m_BirthFinish = {};
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
	IKEObject3d* GetPedestal() { return Pedestal.get(); }
private:
	
	int m_SelectState = SELECT_FIRST;

public:
	void SetSelectState(int SelectState) { m_SelectState = SelectState; };
private:
	int m_BirthTimer = {};
	bool m_Wide = false;
};

