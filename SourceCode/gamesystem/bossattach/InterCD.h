#pragma once
#include "ObjCommon.h"

using namespace std;         //  ���O��Ԏw��
//�����N���X
class InterCD :
public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//������
	virtual bool Initialize() = 0;
	//�X�V
	void Update();
	//�`��
	virtual void Draw(DirectXCommon* dxCommon)override;

	void ImGuiDraw();//ImGui�̕`��

protected:

	virtual void Action() = 0;//�{�X���L�̏���

	virtual void ImGui_Origin() = 0;//�{�X���ꂼ���ImGui

public:
	//gettersetter

private:
private:
};