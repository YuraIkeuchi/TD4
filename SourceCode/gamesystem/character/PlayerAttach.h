#pragma once
#include "ObjCommon.h"
using namespace std;         //  ���O��Ԏw��

//�e�̊��N���X
class PlayerAttach :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	PlayerAttach();
	//������
	bool Initialize()override;
	//�X�V
	void Update()override;
	//�`��
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui����
	void ImGuiDraw();

public:
	//gettersetter
	void SetAfterAngle(const XMFLOAT2& AfterAngle) { m_AfterAngle = AfterAngle; }
	//void SetAfterRot(const float AfterRotY) { m_AfterRotY = AfterRotY; }
public:
	XMFLOAT2 m_AfterAngle = {};//�e�̊p�x
	XMFLOAT2 m_Angle = {};//�e�̊p�x
	float m_AfterRotY = {};
	float m_AddSpeed = {};//�����x
	float m_SinAngle = {};
	float m_SinAngle2 = {};
};