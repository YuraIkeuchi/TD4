#pragma once
#include "ObjCommon.h"
#include <array>
using namespace std;         //  ���O��Ԏw��

//�����N���X
class Note {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Note();
	//������
	bool Initialize();
	//�X�V
	void Update();

	//�`��
	void Draw(DirectXCommon* dxCommon);

	//ImGui����
	void ImGuiDraw();
public:
	//gettersetter

private:
	static const int NOTE_MAX = 3;//�����̍ő吔
private:
	std::array<unique_ptr<IKEObject3d>, NOTE_MAX> m_Object;
	IKEModel* m_Model = nullptr;

	array<XMFLOAT3, NOTE_MAX> m_Position;
	array<XMFLOAT3, NOTE_MAX> m_Rotation;
	array<XMFLOAT3, NOTE_MAX> m_Scale;
	array<XMFLOAT4, NOTE_MAX> m_Color;

	enum NoteType {
		FOLLOW_NOTE,
		DIFF_NOTE,
		CONFU_NOTE,
	};
};