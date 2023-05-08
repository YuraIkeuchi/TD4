#pragma once
#include "Player.h"
#include "IKETexture.h"
#include "CollisionPrimitive.h"
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
	void SetPlayer(Player* player) { _player = player; }

	//gettersetter

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

	virtual void EffecttexDraw(DirectXCommon* dxCommon) = 0;
private:
protected:
	//�e�N���X
	Player* _player;

	int ActionDamage;
	int ActionCool;


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

};

