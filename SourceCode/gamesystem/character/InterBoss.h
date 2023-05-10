#pragma once
#include "IKETexture.h"
#include "CollisionPrimitive.h"
#include "InterBullet.h"
#include "ObjCommon.h"
#include <array>       // �w�b�_�t�@�C���C���N���[�h
using namespace std;         //  ���O��Ԏw��

//�{�X�̊��N���X
class InterBoss :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	

public:

	//������
	virtual bool Initialize() = 0;
	//�X�V
	void Update();
	/// <summary>
	/// �|�[�Y�̂Ƃ�
	/// </summary>
	virtual void Pause() = 0;
	//�`��
	void Draw(DirectXCommon* dxCommon);

	virtual void Action() = 0;//�{�X���L�̏���

	virtual void ImGui_Origin() = 0;//�{�X���ꂼ���ImGui

	void ImGuiDraw();//ImGui�̕`��

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
	//�e�Ƃ̓����蔻��
	void CollideBul(vector<InterBullet*>bullet);

public:
	bool Recv;
};

