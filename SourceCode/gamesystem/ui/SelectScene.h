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
	void Draw_Obj(DirectXCommon* dxcomn);
	/**
	 * \brief ���X���Z�b�g
	 */
	void ResetParam();
private:
	//�y��
	unique_ptr<IKEObject3d>Pedestal=nullptr;
	XMFLOAT3 PedestalRot={0,0,0};

	//�I�u�W�F�N�g���i�X�e�[�W���j
	static constexpr int ObjNum = 7;

	array<unique_ptr<IKETexture>, ObjNum>StageObj={nullptr};
	array<XMFLOAT3, ObjNum>StageObjPos;
	array<float, ObjNum>StageObjRotAngle;
	array<float, ObjNum>StageObjEaseT;

	enum Stage
	{
		FIRST,
		SECOND,
		THIRD,
		FOUR,
		FIVE,
		SIX,
		SEVEN
	}_stages;

	unique_ptr<IKEObject3d>BackSkyDome;
private:
	array<unique_ptr<IKESprite>, 2>ButtonNav_RBLB;
	array<unique_ptr<IKESprite>, ObjNum>StageName;
	array<unique_ptr<IKESprite>, ObjNum>BossIcon;
private:
	//��]�A���O��
	float AngleRot;
	//
	int index;
	//����p
	bool YESorNOflag;

	void SetStage(bool judg,string sceneName);

	void RotPedestal();

public:
	XMFLOAT3 GetPedestalPos() { return Pedestal->GetPosition(); }
	int GetNowIndex() { return index; }
};

