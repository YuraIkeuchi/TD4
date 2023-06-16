#pragma once
#include "IKETexture.h"
#include "IKEObject3d.h"
#include "IKEModel.h"
#include <vector>
#include <any>
#include <memory>
using namespace DirectX;
using namespace std;         //  ���O��Ԏw��
//�_���[�W�G���A
class DamageArea {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	DamageArea(const int Num);

	void Initialize();

	void Update();

	void Draw(DirectXCommon* dxCommon);

	void ImGuiDraw();

private:
	IKEModel* model = nullptr;
	vector<IKEObject3d*> obj;//���f��
	vector<IKETexture*> tex;//�G���A

	vector<XMFLOAT3> m_Position;
	vector<XMFLOAT3> m_Rotation;
	vector<XMFLOAT3> m_Scale;
	vector<XMFLOAT4> m_Color;

	vector<XMFLOAT3> m_TexPosition;
	vector<XMFLOAT3> m_TexRotation;
	vector<XMFLOAT3> m_TexScale;
	vector<XMFLOAT4> m_TexColor;
};