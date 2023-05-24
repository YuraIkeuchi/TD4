#pragma once
#include <DirectXMath.h>
#include <array>   
#include <memory> 
#include "Helper.h"
#include "ParticleEmitter.h"
using namespace std;         //  ���O��Ԏw��
//�G�t�F�N�g�̊��N���X
class InterEffect {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//gettersetter
	bool GetAlive() { return m_Alive; }

	void SetPosition(const XMFLOAT3& position) { m_Position = position; }
public:

	//������
	virtual bool Initialize() = 0;
	//�X�V
	void Update();
	
	virtual void Action() = 0;//�G�t�F�N�g���L�̏���
protected:

	XMFLOAT3 m_Position = { 0.0f,0.0f,0.0f };
	bool m_Alive = false;

	int m_AliveTimer = 0;
};

